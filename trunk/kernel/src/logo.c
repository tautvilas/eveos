#include "logo.h"

#define _ VGA_CL_BLACK
#define X VGA_CL_WHITE
#define Y VGA_CL_RED

int gLogoImage[_LOGO_HEIGHT][_LOGO_WIDTH] = {
    {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
    {_, X, X, X, _, Y, _, _, _, Y, _, X, X, X, _},
    {_, X, _, _, _, Y, Y, _, Y, Y, _, X, _, _, _},
    {_, X, X, X, _, _, Y, _, Y, _, _, X, X, X, _},
    {_, X, _, _, _, _, Y, Y, Y, _, _, X, _, _, _},
    {_, X, X, X, _, _, _, Y, _, _, _, X, X, X, _},
    {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
};

/*
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
*/