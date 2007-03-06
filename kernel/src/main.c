
#include "main.h"


void KERNEL_CALL
os_main()
{
    //vga_clear();
    vga_show_cursor(FALSE);
    vga_set_cursor_pos(0, 4);
    vga_cl_print("Eve successfully switched to P-mode\n", VGA_CL_WHITE, vga_get_bg_color());
    /* TODO vga_set_bg_color() not workint */
    /*
    int i, j;
    for(i = 0; i < LOGO_HEIGHT; i++) {
        for(j = 0; j < LOGO_WIDTH; j++) {
            vga_set_fg_color(gLogoImage[i][j]);
            vga_print_char('X');
        }
        vga_print_char('\n');
    }
    */
    vga_set_fg_color(VGA_CL_LIGHT_GRAY);
    //vga_clear();
    vga_set_bg_color(VGA_CL_BLACK);
    vga_set_fg_color(VGA_CL_LIGHT_GRAY);
    vga_print(gpLogoBig);
    vga_set_fg_color(VGA_CL_LIGHT_GRAY);
    vga_set_bg_color(VGA_CL_BLACK);
    //vga_print("\n");
    vga_print("...at your service.\n");

    //idt_install();

    //for (;;);
    return;
}
