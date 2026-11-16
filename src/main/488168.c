/* Matches newlib's __sfp (findfp.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

struct __sbuf {
    u8 *_base;
    s32 _size;
};

typedef struct __sFILE {
    u8 *_p;
    s32 _r;
    s32 _w;
    s16 _flags;
    s16 _file;
    struct __sbuf _bf;
    s32 _lbfsize;
    void *_cookie;
    void *_read;
    void *_write;
    void *_seek;
    void *_close;
    struct __sbuf _ub;
    u8 *_up;
    s32 _ur;
    u8 _ubuf[3];
    u8 _nbuf[1];
    struct __sbuf _lb;
    s32 _blksize;
    s32 _offset;
    struct _reent *_data;
} FILE;

struct _glue {
    struct _glue *_next;
    s32 _niobs;
    FILE *_iobs;
};

struct _reent {
    s32 _errno;
    u8 pad4[0x34];
    s32 __sdidinit;
    u8 pad3C[0x1D8 - 0x3C];
    struct _glue __sglue;
};

extern struct _glue *__sfmoreglue(struct _reent *, s32);

FILE *__sfp(struct _reent *d) {
    register FILE *fp;
    register s32 n;
    register struct _glue *g;

    if (!d->__sdidinit)
        __sinit(d);
    for (g = &d->__sglue;; g = g->_next) {
        for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
            if (fp->_flags == 0)
                goto found;
        if (g->_next == NULL && (g->_next = __sfmoreglue(d, 4)) == NULL)
            break;
    }
    d->_errno = 12;
    return NULL;

found:
    fp->_flags = 1;
    fp->_p = NULL;
    fp->_w = 0;
    fp->_r = 0;
    fp->_bf._base = NULL;
    fp->_bf._size = 0;
    fp->_lbfsize = 0;
    fp->_file = -1;
    fp->_ub._base = NULL;
    fp->_ub._size = 0;
    fp->_lb._base = NULL;
    fp->_lb._size = 0;
    fp->_data = d;
    return fp;
}
