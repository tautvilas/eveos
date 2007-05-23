#include "main.h"

char* gTestString = "### Kernel sys_write welcomes you! ###\n";

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
    // bss zeroing hack
    // memset((byte_t*) &gBssStart + 0x5000, 0, 0x1000);
    vga_show_cursor(TRUE);
    vga_set_cursor_pos(0, 5); // for not overwriting loader messages
    printf("Eve successfully switched to P-mode with paging enabled\n");
    put_logo();

    idt_install();
    __asm__ __volatile__ ("sti");
    printf("ISRs & IRQs are on-line\n");

    sys_call_table_install();
    printf("System call table installed\n");

    //* IDT test */
    //int x = 4;
    //vga_print_char(x/0);

    timer_install();
    printf("PIT firing rate is %d Hz\n", _TIMER_RATE);

    //a20_enable();
    //printf("A20 gate enabled\n");

    mm_install();
    mm_print_info();

    /*{
        char* p = sbrk(2);
        printf("----------\n");
        printf("p: %x\n", p);
        printf("p[0]: %x\n", (int)*p);
        printf("p[1]: %x\n", (int)p[1]);
        printf("p[4095]: %x\n", (int)p[4095]);
        printf("p[4096]: %x\n", (int)p[4096]);
    }*/

    //mm_print_info();


    /* paging test */
    //dword_t * address;
    //address = (dword_t *) 0x80900000;
    //*address = 0xffff;

    /* kernel call demonstration */

    int len = strlen(gTestString);

    __asm__ __volatile__ ("pusha");
    __asm__ __volatile__ ("mov $4, %eax");  // syscall id (sys_write)
    __asm__ __volatile__ ("mov $1, %ebx");  // stdio
    __asm__ __volatile__ ("movl %0, %%ecx;" :: "m"(gTestString) : "%ecx"); //string offset
    __asm__ __volatile__ ("mov %0, %%edx;" :: "m"(len) : "%edx");  // string length
    __asm__ __volatile__ ("int $69");
    __asm__ __volatile__ ("popa");

	keyboard_install();
    printf("Keyboard is on-line (US layout)\n");
    printf("\n");
	printf("> ");

    for (;;);
    return;
}
