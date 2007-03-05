#include <system.h>

#define _ VGA_CL_YELLOW
#define X VGA_CL_RED

short gLogoImage[LOGO_HEIGHT][LOGO_WIDTH] = {
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

