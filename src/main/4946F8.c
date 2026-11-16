/* Matches libgcc2's __fixdfdi (libgcc2.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s64 __fixdfdi(double a) {
    if (a < 0)
        return -__fixunsdfdi(-a);
    return __fixunsdfdi(a);
}
