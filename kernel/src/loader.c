#include "loader.h"
#include "memmgr.h"
#include "stdio.h"
#include "mem.h"

#define KERNEL_PROCESS_NAME "kernel"

extern dword_t read_cr3();

typedef struct {
    dword_t start;
    size_t text;
    size_t data;
    size_t bss;
} vmm_info_t;

typedef struct {
    dword_t     esp;    //actual position of esp
    dword_t     ss;     //actual stack segment.
    dword_t     kstack; //stacktop of kernel stack
    dword_t     ustack; //stacktop of user stack
    dword_t     cr3;
    bool_t      locked;
    size_t      id;
    size_t      parent;
    size_t      timetorun;
    size_t      sleep;
    size_t      priority;
    vmm_info_t* pVmmInfo;
    char        name[32];
} process_t;

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

typedef struct process_tree_node_t {
    process_t*              pProcess;
    struct process_tree_node_t*    pNext;
    struct process_tree_node_t*    pPrev;
    struct process_tree_node_t*    pParent;
    struct process_tree_node_t*    pFirstChild;
} process_tree_node_t;

size_t gProcessCounter = 0;
size_t gProcessIdCounter = 0;

process_tree_node_t* gpProcessTreeTop;

void KERNEL_CALL
alloc_process_memory(process_t* apProcess)
{
    // void* pKernelPageDir = (void*) read_cr3();
    // void* pTaskPageDir = mm_alloc_page();

    // map kernel at 2gb

    // allocate enough text+data+bss size at bottom

    // allocate stack below 2GB and update process esp
}

void KERNEL_CALL
load_task(void* apOffset, proc_mode_t aMode)
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

    printf("Loading process...\n");
    printf("%vmm start: %x, text size: %d, data size: %d, bss size: %d\n", apOffset, header.text, header.data, header.bss);

    process_t* pProcess = sbrk(sizeof(process_t));
    pProcess->parent = 0;
    pProcess->pVmmInfo = sbrk(sizeof(vmm_info_t));
    pProcess->pVmmInfo->start = 0x00000000;
    pProcess->pVmmInfo->text = header.text;
    pProcess->pVmmInfo->data = header.data;
    pProcess->pVmmInfo->bss = header.bss;

    alloc_process_memory(pProcess);
}

void KERNEL_CALL
multitasking_install(void)
{
    process_t* kernel = sbrk(sizeof(process_t));
    kernel->locked = TRUE;
    kernel->id = gProcessIdCounter;
    kernel->parent = 0;
    char* name = KERNEL_PROCESS_NAME;
    memcpy((byte_t*) kernel->name,(byte_t*) name, strlen(name) + 1);

    gProcessIdCounter++;

    gpProcessTreeTop = sbrk(sizeof(process_tree_node_t));
    gpProcessTreeTop->pParent = NULL;
    //gProcessTreeTop->children = NULL;
    gpProcessTreeTop->pProcess = kernel;

    printf("Multitasking is initialised (just joking;)) \n");
}

void KERNEL_CALL
print_process_tree_node(int aDepth, process_tree_node_t* apNode)
{
    int i;

    process_t* pProcess = apNode->pProcess;
    for (i = 0; i < aDepth; i++)
    {
        printf("  ");
    }
    printf("| id = %d\n", pProcess->id);
    process_tree_node_t* pChild = apNode->pFirstChild;
    if (pChild != NULL)
    {
        do
        {
            print_process_tree_node(aDepth + 1, pChild);
            pChild = pChild->pNext;
        }
        while(pChild != NULL);
    }
}

void KERNEL_CALL
print_process_tree(void)
{
    printf("### PS tree ###\n\n");
    print_process_tree_node(0, gpProcessTreeTop);
    printf("\n");
}
