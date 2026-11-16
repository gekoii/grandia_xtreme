/* Matches libgcc's __adddf3 (config/fp-bit.c), which Sony's ee-gcc calls dpadd. Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct { s32 v[8]; } Buf;

double __pack_d(Buf *);
void __unpack_d(double *, Buf *);
Buf *_fpadd_parts_d(Buf *, Buf *, Buf *);

double dpadd(double arg0, double arg1) {
    Buf a;
    Buf b;
    Buf c;

    __unpack_d(&arg0, &a);
    __unpack_d(&arg1, &b);
    return __pack_d(_fpadd_parts_d(&a, &b, &c));
}
