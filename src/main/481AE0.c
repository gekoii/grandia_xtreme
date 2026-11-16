/* Matches libgcc's __truncdfsf2 (config/fp-bit.c), which Sony's ee-gcc calls dptofp. Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    s32 a, b, c;
    s64 v;
} T;

void __unpack_d(s64 *, T *);

void dptofp(s64 arg0) {
    T t;
    s64 x;
    s32 r;

    x = arg0;
    __unpack_d(&x, &t);
    r = t.v >> 30;
    if (t.v & 0x3FFFFFFF) {
        r |= 1;
    }
    __make_fp(t.a, t.b, t.c, r);
}
