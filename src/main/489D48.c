/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef struct Bigint {
    struct Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    u32 x[1];
} Bigint;

Bigint *Balloc(void *, s32);
Bigint *multadd(void *, Bigint *, s32, s32);

Bigint *s2b(void *ptr, const char *s, s32 nd0, s32 nd, u32 y9) {
    Bigint *b;
    s32 i, k;
    s32 x, y;

    x = (nd + 8) / 9;
    for (k = 0, y = 1; x > y; y <<= 1, k++)
        ;
    b = Balloc(ptr, k);
    b->x[0] = y9;
    b->wds = 1;

    i = 9;
    if (9 < nd0) {
        s += 9;
        do
            b = multadd(ptr, b, 10, *s++ - '0');
        while (++i < nd0);
        s++;
    } else
        s += 10;
    for (; i < nd; i++)
        b = multadd(ptr, b, 10, *s++ - '0');
    return b;
}
