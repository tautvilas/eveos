#include "loader.h"
#include "memmgr.h"
#include "malloc.h"
#include "stdio.h"
#include "mem.h"

#define KERNEL_TASK_NAME "kernel"

extern dword_t      read_cr3();
extern void         write_cr3(dword_t);
extern void         gKernelBase;
extern dword_t      gGdtCsSel;
extern dword_t      gGdtUserCsSel;
extern dword_t      gGdtUserDataSel;
extern dword_t      gKernelEnd;
extern dword_t      gGdtKernelDataSel;

typedef struct {
    dword_t midmag;
    dword_t text;
    dword_t data;
    dword_t bss;
    dword_t syms;
    dword_t entry;
    dword_t trsize;
    dword_t drsize;
} aout_exec_t;

task_t* gpTopTask = NULL;
task_t* gpActiveTask = NULL;
task_t* gpForegroundTask = NULL;
task_ring_node_t* gpActiveTaskRingNode = NULL;

static size_t gsTaskCounter = 0;
static size_t gsTaskIdCounter = 0;

dword_t gPingTaskOffset;
dword_t gKernelTaskOffset;
dword_t gKernelPageDir;

task_tree_node_t* gpTaskTreeTop;
dword_t gNextTaskOffset = 0;
dword_t gKernelCr3 = 0;

uint_t gpPriorityTimes[3] = {10, 20, 50};

task_ring_node_t* KERNEL_CALL
load_task(void* apOffset, task_ring_node_t* apParent, mm_access_t aAccess, priority_t aPriority, bool_t aOnTop)
{
    write_cr3(gKernelCr3);

    aout_exec_t header;
    dword_t* pOffset;
    pOffset = apOffset;
    header.midmag = (dword_t) (*pOffset++);
    header.text = (dword_t) (*pOffset++);
    header.data = (dword_t) (*pOffset++);
    header.bss = (dword_t) (*pOffset++);
    header.syms = (dword_t) (*pOffset++);
    header.entry = (dword_t) (*pOffset++);
    header.trsize = (dword_t) (*pOffset++);
    header.drsize = (dword_t) (*pOffset);

    if(gNextTaskOffset == (dword_t) apOffset)
    {
        gNextTaskOffset += header.data + header.text + sizeof(aout_exec_t);
    }

    BRAG("*** Kernel is loading task... ***\n");
    BRAG("Bin start: %x, entry: %d, text size: %d, data size: %d, bss size: %d\n", apOffset, header.entry,
            header.text, header.data, header.bss);

    task_t* pTask = sbrk(sizeof(task_t));
    pTask->parent = apParent->pTask->id;
    pTask->vm_info.start = 0;
    pTask->vm_info.entry = header.entry;
    pTask->vm_info.text_size = header.text;
    pTask->vm_info.data_size = header.data;
    pTask->vm_info.bss_size = header.bss;
    pTask->vm_info.header_size = sizeof(aout_exec_t);
    pTask->id = gsTaskIdCounter;
    pTask->priority = aPriority;
    pTask->timetorun = gpPriorityTimes[aPriority];

    DUMP(aOnTop);
    if (aOnTop)
        gpTopTask   = pTask;

    gsTaskIdCounter++;
    gsTaskCounter++;


    // alloc task memory
    pTask->page_dir = mm_alloc_task(&pTask->vm_info, apOffset, aAccess);
    DUMP(pTask->page_dir);

    pTask->ustack = 2U * GIGABYTE - 20 * sizeof(uint_t);
    // swapping page dirs might be dangerous
    if(aAccess == ACC_SUPER)
    {
        dword_t kernel_page_dir = read_cr3();
        __asm__ __volatile__ ("cli");

        write_cr3(pTask->page_dir);

        // prepare RING 0 task stack
        // tribute to infinity
        dword_t* pStack = (dword_t*)pTask->ustack;
        pStack--;
        *(pStack)-- = 0x202;    //eflags: enable interrupts and set reserved bit to 1
        *(pStack)-- = gGdtCsSel;
        *(pStack)-- = pTask->vm_info.entry;
        *(pStack)-- = 0x00;     // error code
        *(pStack)-- = 0x00;     // isr num
        *(pStack)-- = 0x00;
        //*(pStack)-- = (dword_t)((dword_t*) pTask->ustack) - 17;
        *(pStack)-- = 0x00;
        *(pStack)-- = 0x00;
        *(pStack)-- = 0x00;
        *(pStack)-- = 0x00;
        *(pStack)-- = 0x00;
        *(pStack)-- = 0x00;
        *(pStack)-- = 0x00;
        *(pStack)-- = 0x10;
        *(pStack)-- = 0x10;
        *(pStack)-- = 0x10;
        *(pStack)   = 0x10;

        write_cr3(kernel_page_dir);
        __asm__ __volatile__ ("sti");
        pTask->esp = (dword_t)pStack;
    }
    else    // user privilege app
    {

        regs_t* pStack = malloc(sizeof(regs_t));
        memset(pStack, 0, sizeof(regs_t));

        pStack->eip     =   pTask->vm_info.entry;
        pStack->eflags  =   0x202;
        pStack->cs      =   gGdtUserCsSel   | 3;
        pStack->ss      =   gGdtUserDataSel | 3;
        pStack->useresp =   pTask->ustack;

        pStack->es      =   gGdtUserDataSel;
        pStack->ds      =   gGdtUserDataSel;
        pStack->fs      =   gGdtUserDataSel;
        pStack->gs      =   gGdtUserDataSel;

        pTask->esp      = (dword_t)pStack;
        pTask->kstack   = (dword_t)pStack + sizeof(regs_t);
        pTask->ss       = gGdtUserDataSel;
    }

    // put the task into task ring
    task_ring_node_t* pNode = malloc(sizeof(task_ring_node_t));
    pNode->pTask = pTask;
    pNode->pNext = gpActiveTaskRingNode->pNext;
    pNode->pNext->pPrev = pNode;
    pNode->pPrev = gpActiveTaskRingNode;
    gpActiveTaskRingNode->pNext = pNode;

    // put task into task tree node

    /* task_tree_node_t* pTreeNode = malloc(sizeof(task_tree_node_t));
    task_tree_node_t* pPrevChild = NULL;
    task_tree_node_t* pParentTreeNode = apParent->pTreeNode;
    task_tree_node_t* pChild = pParentTreeNode->pFirstChild;
    while (pChild != NULL)
    {
        pPrevChild = pChild;
        pChild = pChild->pNext;
    }
    DUMP(0x000);
    if(pPrevChild) pPrevChild->pNext = pTreeNode;
    else pParentTreeNode->pFirstChild = pTreeNode;
    pTreeNode->pPrev = pPrevChild;
    pTreeNode->pNext = NULL;

    pTreeNode->pFirstChild = NULL;
    pTreeNode->pTask = pTask;

    pNode->pTreeNode = pTreeNode; */

    BRAG("*** Kernel has ended loading task... Number of tasks running: %d ***\n", gsTaskCounter);

    return pNode;
}

task_ring_node_t* KERNEL_CALL
multitasking_install(void)
{
    task_t* pKernel = malloc(sizeof(task_t));
    pKernel->locked = FALSE;
    pKernel->id = gsTaskIdCounter;
    pKernel->parent = 0;
    pKernel->page_dir = read_cr3();
    pKernel->priority = PRIOR_HIGH;

    gsTaskIdCounter++;
    gsTaskCounter++;

    gKernelCr3 = pKernel->page_dir;
    // from here multitasking starts
    gpActiveTask = pKernel;
    gpForegroundTask = pKernel;

    task_ring_node_t* pNode = malloc(sizeof(task_ring_node_t));
    pNode->pTask = pKernel;
    pNode->pNext = pNode;
    pNode->pPrev = pNode;

    gpActiveTaskRingNode = pNode;

    gNextTaskOffset = (dword_t) &gKernelEnd;

    // tree setup
    task_tree_node_t* pTreeNode = malloc(sizeof(task_tree_node_t));
    pTreeNode->pTask = pKernel;
    pTreeNode->pNext = NULL;
    pTreeNode->pPrev = NULL;
    pTreeNode->pFirstChild = NULL;

    pNode->pTreeNode = pTreeNode;

    gpTaskTreeTop = pTreeNode;

    return pNode;
}

void KERNEL_CALL
print_task_tree_node(int aDepth, task_tree_node_t* apNode)
{
    int i;

    task_t* pTask = apNode->pTask;
    for (i = 0; i < aDepth; i++)
    {
        printf("--");
    }
    printf("| id = %d\n", pTask->id);
    task_tree_node_t* pChild = apNode->pFirstChild;
    while (pChild != NULL)
    {
        print_task_tree_node(aDepth + 1, pChild);
        //DUMP(pChild);
        pChild = pChild->pNext;
    }
}

void KERNEL_CALL
print_task_tree(void)
{
    printf("### PS tree start ###\n\n");
    print_task_tree_node(0, gpTaskTreeTop);
    printf("\n### PS tree end ###\n");
}
