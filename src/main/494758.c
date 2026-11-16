/* Matches libgcc2's __fixunsdfdi (libgcc2.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s64 __fixunsdfdi(double a) {
    double b;
    u64 v;

    if (a < 0)
        return 0;

    b = (a / 4294967296.0);
    v = (u32) dptoul(b);
    v <<= 32;
    a -= (double) v;
    if (a < 0)
        v -= (u32) dptoul(-a);
    else
        v += (u32) dptoul(a);
    return v;
}
