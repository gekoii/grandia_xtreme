/* Matches newlib's fflush (fflush.c). Third-party library code: see README.md, "Third-party library code". */
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

struct _reent {
    s32 _errno;
    u8 pad4[0x34];
    s32 __sdidinit;
};

extern struct _reent *_impure_ptr;
extern s32 _fwalk(struct _reent *, s32 (*)(FILE *));

s32 fflush(register FILE *fp) {
    register u8 *p;
    register s32 n, t;

    if (fp == NULL)
        return _fwalk(_impure_ptr, fflush);

    do {
        if (fp->_data == 0)
            fp->_data = _impure_ptr;
        if (!fp->_data->__sdidinit)
            __sinit(fp->_data);
    } while (0);

    t = fp->_flags;
    if ((t & 0x8) == 0 || (p = fp->_bf._base) == NULL)
        return 0;
    n = fp->_p - p;

    fp->_p = p;
    fp->_w = t & (0x1 | 0x2) ? 0 : fp->_bf._size;

    while (n > 0) {
        t = (*(s32 (*)(void *, char *, s32))fp->_write)(fp->_cookie, (char *)p, n);
        if (t <= 0) {
            fp->_flags |= 0x40;
            return -1;
        }
        p += t;
        n -= t;
    }
    return 0;
}
