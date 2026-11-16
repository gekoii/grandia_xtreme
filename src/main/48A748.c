/* Matches newlib's b2d (mprec.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct _Bigint {
    struct _Bigint *_next;
    s32 _k, _maxwds, _sign, _wds;
    u32 _x[1];
} _Bigint;

union double_union {
    double d;
    u32 i[2];
};

#define word0(x) (x.i[1])
#define word1(x) (x.i[0])
#define Exp_1 0x3ff00000
#define Ebits 11


double b2d(_Bigint *a, s32 *e) {
    u32 *xa, *xa0, w, y, z;
    s32 k;
    union double_union d;
#define d0 word0(d)
#define d1 word1(d)

    xa0 = a->_x;
    xa = xa0 + a->_wds;
    y = *--xa;
    k = hi0bits(y);
    *e = 32 - k;
    if (k < Ebits) {
        d0 = Exp_1 | y >> (Ebits - k);
        w = xa > xa0 ? *--xa : 0;
        d1 = y << ((32 - Ebits) + k) | w >> (Ebits - k);
        goto ret_d;
    }
    z = xa > xa0 ? *--xa : 0;
    if (k -= Ebits) {
        d0 = Exp_1 | y << k | z >> (32 - k);
        y = xa > xa0 ? *--xa : 0;
        d1 = z << k | y >> (32 - k);
    } else {
        d0 = Exp_1 | y;
        d1 = z;
    }
ret_d:
    return d.d;
}
