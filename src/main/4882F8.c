/* Matches newlib's fread (fread.c). Third-party library code: see README.md, "Third-party library code". */
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

extern void *memcpy(void *, const void *, u32);

u32 fread(void *buf, u32 size, u32 count, FILE *fp) {
    register u32 resid;
    register char *p;
    register s32 r;
    u32 total;

    if ((resid = count * size) == 0)
        return 0;
    if (fp->_r < 0)
        fp->_r = 0;
    total = resid;
    p = buf;
    while (resid > (r = fp->_r)) {
        (void)memcpy((void *)p, (void *)fp->_p, (u32)r);
        fp->_p += r;
        p += r;
        resid -= r;
        if (__srefill(fp)) {
            return (total - resid) / size;
        }
    }
    (void)memcpy((void *)p, (void *)fp->_p, resid);
    fp->_r -= resid;
    fp->_p += resid;
    return count;
}
