#include "utils.h"

unsigned int getPaddingDiff(unsigned int width)
{
    unsigned int diff = 0;
    unsigned int width_with_padd = width;
    while (width_with_padd % 4 != 0)
        width_with_padd++;
    diff = width_with_padd - width;
    return diff;
}

int Min(int a, int b)
{
    return (a <= b) ? a : b;
}

int Max(int a, int b)
{
    return (a >= b) ? a : b;
}
