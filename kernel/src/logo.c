#include "logo.h"
#include "vga.h"

//#define _ VGA_CL_YELLOW
//#define X VGA_CL_RED

// :BUG: gd 2007-03-05: got compiler errors:
// ~~~ Compiling logo.o from logo.c
// C:\DOCUME~1\GoniX\LOCALS~1\Temp/cc86aaaa.s: Assembler messages:
// C:\DOCUME~1\GoniX\LOCALS~1\Temp/cc86aaaa.s:4: Warning: alignment too large: 31 a
// ssumed
// C:\DOCUME~1\GoniX\LOCALS~1\Temp/cc86aaaa.s:140: Fatal error: cannot write to out
// put file
// make[1]: *** [logo.o] Error 1
/*
int gLogoImage[LOGO_HEIGHT][LOGO_WIDTH] = {
    {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
    {_, X, X, X, _, X, _, _, _, X, _, X, X, X, _},
    {_, X, _, _, _, X, _, _, _, X, _, X, _, _, _},
    {_, X, _, _, _, _, X, _, X, _, _, X, _, _, _},
    {_, X, X, X, _, _, X, _, X, _, _, X, X, X, _},
    {_, X, _, _, _, _, X, X, X, _, _, X, _, _, _},
    {_, X, _, _, _, _, _, X, _, _, _, X, _, _, _},
    {_, X, X, X, _, _, _, X, _, _, _, X, X, X, _},
    {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
};
*/


char* gpLogoBig =
        "�������������������������������������� ��\n"
        "�� � ������������������������������� � ��\n"
        "� ������������������������������������� �\n"
        " ��������������������������������������� \n"
        "���������������������������������������۱\n"
        "۲�� ��������������۱��۲����������� ����\n"
        " ������������������   ���߲������������� \n"
        "  ��  ������� ��� ��������� �������  ��  \n"
        "��   �� ���� ��������������� ���� ��  ���\n"
        "������ �    ���������������� ����� �� ���\n"
        "�۲����������������۱������������� ������\n"
        "�������� ����������� ����������� ���۲���\n"
        "�������۱ ���������� ���������� �۲������\n"
        "�������� ����������������������� ��������\n";

