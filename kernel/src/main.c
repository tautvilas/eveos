#include "main.h"

void KERNEL_CALL
os_main()
{
    //vga_clear();
    vga_show_cursor(TRUE);
    vga_set_cursor_pos(0, 4);
    printf("Eve successfully switched to P-mode\n");
    int i, j;
    for(i = 0; i < LOGO_HEIGHT; i++) {
        for(j = 0; j < LOGO_WIDTH; j++) {
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
        vga_print("...at your service.\n");
    */
    vga_set_bg_color(VGA_CL_BLACK);
    vga_set_fg_color(VGA_CL_LIGHT_GRAY);

    idt_install();
    int x = 4;
    vga_print_char(x/0);

    //for (;;);
    return;
}
