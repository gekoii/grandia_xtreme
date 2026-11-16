/* Matches newlib's Balloc (mprec.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct _Bigint {
    struct _Bigint *_next;
    s32 _k, _maxwds, _sign, _wds;
    u32 _x[1];
} _Bigint;

struct _reent {
    u8 pad0[0x4C];
    _Bigint **_freelist;
};


_Bigint *Balloc(struct _reent *ptr, s32 k) {
    s32 x;
    _Bigint *rv;

    if (ptr->_freelist == NULL) {
        ptr->_freelist = (_Bigint **)_calloc_r(ptr, sizeof(_Bigint *), 16);
        if (ptr->_freelist == NULL) {
            return NULL;
        }
    }

    if ((rv = ptr->_freelist[k]) != 0) {
        ptr->_freelist[k] = rv->_next;
    } else {
        x = 1 << k;
        rv = (_Bigint *)_calloc_r(ptr, 1, sizeof(_Bigint) + (x - 1) * sizeof(rv->_x));
        if (rv == NULL)
            return NULL;
        rv->_k = k;
        rv->_maxwds = x;
    }
    rv->_sign = rv->_wds = 0;
    return rv;
}
