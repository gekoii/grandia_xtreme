/* Matches libgcc's __floatdidf (libgcc2.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef s32 SItype;
typedef u32 USItype;
typedef s64 DItype;
typedef double DFtype;

#define WORD_SIZE 32
#define HIGH_HALFWORD_COEFF (((u64)1) << (WORD_SIZE / 2))
#define HIGH_WORD_COEFF (((u64)1) << WORD_SIZE)

DFtype __floatdidf(DItype u) {
    DFtype d;

    d = (SItype)(u >> WORD_SIZE);
    d *= HIGH_HALFWORD_COEFF;
    d *= HIGH_HALFWORD_COEFF;
    d += (USItype)(u & (HIGH_WORD_COEFF - 1));

    return d;
}
