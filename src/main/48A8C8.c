/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct Bigint {
    struct Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    u32 x[1];
} Bigint;

typedef union {
    s64 d;
    u32 i[2];
} DU;

extern Bigint *Balloc(void *, s32);

Bigint *d2b(void *ptr, s64 _d, s32 *e, s32 *bits) {
    DU d;
    Bigint *b;
    s32 de;
    s32 i;
    s32 k;
    u32 *x;
    u32 y;
    u32 z;

    d.d = _d;
    b = Balloc(ptr, 1);
    x = b->x;
    z = d.i[1] & 0xFFFFF;
    d.i[1] &= 0x7FFFFFFF;
    if ((de = (s32)(d.i[1] >> 20)) != 0) {
        z |= 0x100000;
    }
    if ((y = d.i[0]) != 0) {
        if ((k = lo0bits(&y)) != 0) {
            x[0] = y | z << (32 - k);
            z >>= k;
        } else {
            x[0] = y;
        }
        i = b->wds = (x[1] = z) ? 2 : 1;
    } else {
        k = lo0bits(&z);
        x[0] = z;
        i = b->wds = 1;
        k += 32;
    }
    if (de) {
        *e = de - 1023 - 52 + k;
        *bits = 53 - k;
    } else {
        *e = de - 1023 - 52 + 1 + k;
        *bits = 32 * i - hi0bits(x[i - 1]);
    }
    return b;
}
