/* Matches libgcc's __fixunssfdi (libgcc2.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef u32 USItype;
typedef s64 DItype;
typedef u64 UDItype;
typedef f32 SFtype;
typedef double DFtype;

#define WORD_SIZE 32
#define HIGH_WORD_COEFF (((UDItype)1) << WORD_SIZE)

DFtype func_00480D48(DItype);

UDItype __fixunssfdi(SFtype original_a) {
    DFtype a = original_a;
    DFtype b;
    DFtype t;
    UDItype v;

    if (a < 0)
        return 0;

    b = (a / HIGH_WORD_COEFF);
    v = dptoul(b);
    v <<= WORD_SIZE;
    if ((DItype)v >= 0) {
        t = func_00480D48(v);
    } else {
        t = func_00480D48((v & 1) | (v >> 1));
        t = t + t;
    }
    a -= t;
    if (a < 0)
        v -= dptoul(-a);
    else
        v += dptoul(a);
    return v;
}
