/* Matches newlib's _fwalk (fwalk.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct __sFILE {
    u8 pad0[0xC];
    s16 _flags;
    u8 padE[0x58 - 0xE];
} FILE;

struct _glue {
    struct _glue *_next;
    s32 _niobs;
    FILE *_iobs;
};

struct _reent {
    u8 pad0[0x1D8];
    struct _glue __sglue;
};

s32 _fwalk(struct _reent *ptr, s32 (*function)(FILE *)) {
    register FILE *fp;
    register s32 n, ret = 0;
    register struct _glue *g;

    for (g = &ptr->__sglue; g != NULL; g = g->_next)
        for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
            if (fp->_flags != 0)
                ret |= (*function)(fp);
    return ret;
}
