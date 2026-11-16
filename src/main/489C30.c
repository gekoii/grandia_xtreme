/* Matches newlib's multadd (mprec.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct _Bigint {
    struct _Bigint *_next;
    s32 _k, _maxwds, _sign, _wds;
    u32 _x[1];
} _Bigint;

struct _reent;
extern _Bigint *Balloc(struct _reent *, s32);
extern void *memcpy(void *, const void *, u32);

#define Bcopy(x, y) memcpy((char *)&x->_sign, (char *)&y->_sign, y->_wds * sizeof(s32) + 2 * sizeof(s32))

_Bigint *multadd(struct _reent *ptr, _Bigint *b, s32 m, s32 a) {
    s32 i, wds;
    u32 *x, y;
    u32 xi, z;
    _Bigint *b1;

    wds = b->_wds;
    x = b->_x;
    i = 0;
    do {
        xi = *x;
        y = (xi & 0xffff) * m + a;
        z = (xi >> 16) * m + (y >> 16);
        a = (s32)(z >> 16);
        *x++ = (z << 16) + (y & 0xffff);
    } while (++i < wds);
    if (a) {
        if (wds >= b->_maxwds) {
            b1 = Balloc(ptr, b->_k + 1);
            Bcopy(b1, b);
            Bfree(ptr, b);
            b = b1;
        }
        b->_x[wds++] = a;
        b->_wds = wds;
    }
    return b;
}
