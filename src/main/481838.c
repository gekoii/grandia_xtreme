/* Matches libgcc's __cmpdf2 (config/fp-bit.c), which Sony's ee-gcc calls dpcmp. Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

void __unpack_d(s64 *, void *);
void __fpcmp_parts_d(void *, void *);

void dpcmp(s64 arg0, s64 arg1) {
    u8 a[32];
    u8 b[32];
    __unpack_d(&arg0, a);
    __unpack_d(&arg1, b);
    __fpcmp_parts_d(a, b);
}
