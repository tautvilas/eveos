#include "main.h"

extern void gKernelEnd;

//TODO:zv:move this function the fuck out of this fucken file

static void KERNEL_CALL
put_logo()
{
    int i, j;
    //vga_clear();
    for(i = 0; i < _LOGO_HEIGHT; i++) {
        for(j = 0; j < _LOGO_WIDTH; j++) {
            vga_set_bg_color(gLogoImage[i][j]);
            vga_print_char(' ');
        }
        vga_print_char('\n');
    }
    /* explicit content :> */
    /*
        vga_set_bg_color(VGA_CL_BLACK);
        vga_set_fg_color(VGA_CL_LIGHT_GRAY);
        vga_print(gpLogoBig);
        vga_print("\n");
    */
    vga_set_bg_color(VGA_CL_BLACK);
    vga_set_fg_color(VGA_CL_LIGHT_GRAY);
}


void KERNEL_CALL
os_main()
{
    /* TODO:zv: change to vga_install */
    vga_show_cursor(TRUE);
    vga_set_cursor_pos(0, 5); // for not overwriting loader messages
    printf("Eve successfully switched to P-mode with paging enabled\n");
    put_logo();

    idt_install();
    __asm__ __volatile__ ("sti");
    printf("ISRs & IRQs are on-line\n");

    sys_call_table_install();
    printf("System call table installed\n");

    timer_install();
    printf("PIT firing rate is %d Hz\n", _TIMER_RATE);

    mm_install();
    mm_print_info();

    /*{
        char* p;

        printf("-----------\n");
        DBG_DUMP(p = sbrk(2));
        DBG_DUMP(sbrk(0));
        DBG_DUMP(*p);
        DBG_DUMP(p[1]);
        DBG_DUMP(p[4095]); // possable page fault
        //DBG_DUMP(p[4096]); // page fault for sure

        printf("-----------\n");
        DBG_DUMP(p = sbrk(4 * MEGABYTE));
        DBG_DUMP(sbrk(0));
        DBG_DUMP(p[0]);
        DBG_DUMP(p[MEGABYTE]);
        DBG_DUMP(p[4 * MEGABYTE]); // possable page fault
        //DBG_DUMP(p[5 * MEGABYTE]); // page fault for sure

        printf("-----------\n");
        DBG_DUMP(sbrk(-4 * MEGABYTE));
        DBG_DUMP(p = sbrk(0));
        DBG_DUMP(p[0]);     // possable page fault
        //DBG_DUMP(p[MEGABYTE]);  // page fault for sure

        DBG_DUMP(sbrk(-2));
        DBG_DUMP(p = sbrk(0));
        //DBG_DUMP(p[0]); // page fault for sure
    }*/

    /* kernel call demonstration */

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

    /*
	keyboard_install();
    printf("Keyboard is on-line (US layout)\n");
    printf("\n");
	printf("> ");
    */

    //multitasking_install();
    //print_process_tree();
    load_task(&gKernelEnd, ACC_USER);

    for (;;);
    return;
}
