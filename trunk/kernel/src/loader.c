#include "loader.h"
#include "memmgr.h"
#include "stdio.h"
#include "mem.h"

#define KERNEL_TASK_NAME "kernel"

extern dword_t read_cr3();
extern void gKernelBase;
extern dword_t gGdtCsSel;
extern void write_cr3(dword_t);

typedef struct {
    dword_t         esp;    //actual position of esp
    dword_t         ss;     //actual stack segment.
    dword_t         kstack; //stacktop of kernel stack
    dword_t         ustack; //stacktop of user stack
    dword_t         page_dir;
    bool_t          locked;
    size_t          id;
    size_t          parent;
    size_t          timetorun;
    size_t          sleep;
    size_t          priority;
    mm_task_mem_t   vm_info;
    char            name[32];
} task_t;

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

typedef struct task_tree_node_t {
    task_t*              pTask;
    struct task_tree_node_t*    pNext;
    struct task_tree_node_t*    pPrev;
    struct task_tree_node_t*    pParent;
    struct task_tree_node_t*    pFirstChild;
} task_tree_node_t;

size_t gTaskCounter = 0;
size_t gTaskIdCounter = 0;

task_tree_node_t* gpTaskTreeTop;

/*void KERNEL_CALL
alloc_process_memory(task_t* apTask)
{
    size_t table_id;
    dword_t* pKernelPageDir = (void*) read_cr3();
    dword_t* pTaskPageDir = mm_alloc_page();

    printf("task page dir: %x\n", pTaskPageDir);

    pTaskPageDir[0] = 0;

    //for (table_id = 0; table_id < MM_PAGE_SIZE / 4; table_id++)
    //{
    //    pTaskPageDir[table_id] = 0;
    //}

    //for (
    //        table_id = (dword_t)&gKernelBase / MEGABYTE / 4;
    //        table_id < MM_PAGE_SIZE / 4;
    //        table_id++
    //    )
    //{
    //    pTaskPageDir[table_id] = pKernelPageDir[table_id];
    //}

    ///mm_task_mem_t* vm_info = apTask->vm_info;
    //size_t static_mem_size = vm_info->data + pVmInfo->text + pVmInfo->bss;
    // allocate stack below 2GB and update process esp
}*/

void KERNEL_CALL
load_task(void* apOffset, mm_access_t aAccess)
{
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

    printf("Loading task...\n");
    printf("Binary start: %x, text size: %d, data size: %d, bss size: %d\n", apOffset, header.text, header.data, header.bss);

    task_t* pTask = sbrk(sizeof(task_t));
    pTask->parent = 0;
    pTask->vm_info.start = 0x00000000;
    pTask->vm_info.text_size = header.text + 32;
    pTask->vm_info.data_size = header.data;
    pTask->vm_info.bss_size = header.bss;

    pTask->page_dir = mm_alloc_task(&pTask->vm_info, apOffset, aAccess);
    printf("Task page directory: %x\n", pTask->page_dir);
    pTask->esp = 2U * GIGABYTE - sizeof(uint_t);

    dword_t* stack = sbrk(100 * sizeof(dword_t*));
    stack[99] = 0x0202;
    stack[98] = gGdtCsSel;
    stack[97] = pTask->vm_info.start;
    stack[96] = 0x00;
    stack[95] = 0x00;
    stack[94] = 0x00;
    stack[93] = 0x00;
    stack[92] = 0x00;
    stack[91] = 0x00;
    stack[90] = 0x00;
    stack[89] = 0x00;
    stack[88] = 0x10;
    stack[87] = 0x10;
    stack[86] = 0x10;
    stack[85] = 0x10;
    write_cr3(pTask->page_dir);
    DBG_DUMP(*(dword_t*)0x0);
    //__asm__ __volatile__ ("movl %0, %%esp;" :: "r"(stack + 85) : "%esp"); //string offset
    __asm__ __volatile__ ("call 32");
}

void KERNEL_CALL
multitasking_install(void)
{
    task_t* kernel = sbrk(sizeof(task_t));
    kernel->locked = TRUE;
    kernel->id = gTaskIdCounter;
    kernel->parent = 0;
    char* name = KERNEL_TASK_NAME;
    memcpy((byte_t*) kernel->name,(byte_t*) name, strlen(name) + 1);

    gTaskIdCounter++;

    gpTaskTreeTop = sbrk(sizeof(task_tree_node_t));
    gpTaskTreeTop->pParent = NULL;
    //gTaskTreeTop->children = NULL;
    gpTaskTreeTop->pTask = kernel;

    printf("Multitasking is initialised (just joking;)) \n");
}

void KERNEL_CALL
print_task_tree_node(int aDepth, task_tree_node_t* apNode)
{
    int i;

    task_t* pTask = apNode->pTask;
    for (i = 0; i < aDepth; i++)
    {
        printf("  ");
    }
    printf("| id = %d\n", pTask->id);
    task_tree_node_t* pChild = apNode->pFirstChild;
    if (pChild != NULL)
    {
        do
        {
            print_task_tree_node(aDepth + 1, pChild);
            pChild = pChild->pNext;
        }
        while(pChild != NULL);
    }
}

void KERNEL_CALL
print_task_tree(void)
{
    printf("### PS tree ###\n\n");
    print_task_tree_node(0, gpTaskTreeTop);
    printf("\n");
}
