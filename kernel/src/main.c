#include "main.h"

extern void gKernelEnd;

void KERNEL_CALL
os_main()
{
    vga_install();
    BRAG("Eve successfully switched to P-mode with paging enabled\n");
    put_logo();

    idt_install();
    BRAG("ISRs & IRQs are on-line\n");

    sys_call_table_install();
    BRAG("System call table installed\n");

    /*{
        int len = strlen(gTestString);

        __asm__ __volatile__ ("pusha");
        __asm__ __volatile__ ("mov $4, %eax");  // syscall id (sys_write)
        __asm__ __volatile__ ("mov $1, %ebx");  // stdio
        __asm__ __volatile__ ("movl %0, %%ecx;" :: "m"(gTestString) : "%ecx"); //string offset
        __asm__ __volatile__ ("mov %0, %%edx;" :: "m"(len) : "%edx");  // string length
        __asm__ __volatile__ ("int $69");
        __asm__ __volatile__ ("popa");
    }*/

    timer_install();
    BRAG("PIT firing rate is %d Hz\n", _TIMER_RATE);

    mm_install();
    mm_print_info();

    /*{
        char* p;

        SEPARATE
        DBG_DUMP(p = sbrk(2));
        DBG_DUMP(sbrk(0));
        DBG_DUMP(*p);
        DBG_DUMP(p[1]);
        DBG_DUMP(p[4095]); // possable page fault
        //DBG_DUMP(p[4096]); // page fault for sure

        SEPARATE
        DBG_DUMP(p = sbrk(4 * MEGABYTE));
        DBG_DUMP(sbrk(0));
        DBG_DUMP(p[0]);
        DBG_DUMP(p[MEGABYTE]);
        DBG_DUMP(p[4 * MEGABYTE]); // possable page fault
        //DBG_DUMP(p[5 * MEGABYTE]); // page fault for sure

        SEPARATE
        DBG_DUMP(sbrk(-4 * MEGABYTE));
        DBG_DUMP(p = sbrk(0));
        DBG_DUMP(p[0]);     // possable page fault
        //DBG_DUMP(p[MEGABYTE]);  // page fault for sure

        DBG_DUMP(sbrk(-2));
        DBG_DUMP(p = sbrk(0));
        //DBG_DUMP(p[0]); // page fault for sure
    }*/

    /*{
        char* p;
        SEPARATE;
        DUMP(sbrk(0));
        DUMP(p = malloc(2));
        DUMP(sbrk(0));
        DUMP(*p);
        DUMP(p[1]);
        //DUMP(p[4095]); // possable page fault
        //DUMP(p[4096]); // page fault for sure

        char* q;
        SEPARATE;
        DUMP(q = malloc(4 * MEGABYTE));
        DUMP(sbrk(0));
        DUMP(q[0]);
        DUMP(q[MEGABYTE]);
        DUMP(q[4 * MEGABYTE]); // possable page fault
        //DUMP(p[5 * MEGABYTE]); // page fault for sure

        SEPARATE;
        MARK(free(q));
        DUMP(sbrk(0));
        //DUMP(q[0]);     // possable page fault
        //DUMP(p[MEGABYTE]);  // page fault for sure

        MARK(free(p));
        DUMP(sbrk(0));
        //DUMP(p[0]); // page fault for sure
    }*/

    //multitasking_install();
    //print_process_tree();
    load_task(&gKernelEnd, ACC_USER);

    /*
	keyboard_install();
    BRAG("Keyboard is on-line (US layout)\n");
    */

    for (;;);
    return;
}
