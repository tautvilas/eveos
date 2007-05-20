#include "main.h"

int gTmp = 0;

extern void * sys_stack;
extern void * gKernelStart;

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
    vga_show_cursor(TRUE);
    vga_set_cursor_pos(0, 4); // for not overwriting loader messages
    printf("Eve successfully switched to P-mode with paging\n");
    put_logo();

    idt_install();
    __asm__ __volatile__ ("sti");
    printf("ISRs & IRQs are on-line\n");

    //* IDT test */
    //int x = 4;
    //vga_print_char(x/0);

    timer_install();
    printf("PIT firing rate is %d Hz\n", _TIMER_RATE);
    a20_enable();
    printf("A20 gate enabled\n");
    //mm_install();
    printf("Memory manager enabled\n");
	//paging_install();
    printf("Paging is enabled\n");
    //mm_print_info();
    

    /* paging test */
    //dword_t * address;
    //address = (dword_t *) 0x80000000;
    //*address = 0xffff;

    printf("%x\n" , &sys_stack);

	keyboard_install();
    printf("Keyboard is on-line (US layout)\n");
    printf("\n");
	printf("> ");

    for (;;);
    return;
}
