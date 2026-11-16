/* Matches newlib's __sfvwrite (fvwrite.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef u32 size_t;

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

struct __siov {
    const void *iov_base;
    size_t iov_len;
};

struct __suio {
    struct __siov *uio_iov;
    s32 uio_iovcnt;
    s32 uio_resid;
};

extern s32 fflush(FILE *);
extern void *memmove(void *, const void *, size_t);
extern void *memchr(const void *, s32, size_t);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define COPY(n) (void)memmove((void *)fp->_p, (void *)p, (size_t)(n))

#define GETIOV(extra_work) \
    while (len == 0) {     \
        extra_work;        \
        p = iov->iov_base; \
        len = iov->iov_len; \
        iov++;             \
    }

#define cantwrite(fp) ((((fp)->_flags & 0x8) == 0 || (fp)->_bf._base == NULL) && __swsetup(fp))

s32 __sfvwrite(register FILE *fp, register struct __suio *uio) {
    register size_t len;
    register const char *p;
    register struct __siov *iov;
    register s32 w, s;
    char *nl;
    s32 nlknown, nldist;

    if ((len = uio->uio_resid) == 0)
        return 0;

    if (cantwrite(fp))
        return -1;

    iov = uio->uio_iov;
    len = 0;
    if (fp->_flags & 0x2) {
        do {
            GETIOV(;);
            w = (*(s32 (*)(void *, const char *, s32))fp->_write)(fp->_cookie, p, MIN(len, 1024));
            if (w <= 0)
                goto err;
            p += w;
            len -= w;
        } while ((uio->uio_resid -= w) != 0);
    } else if ((fp->_flags & 0x1) == 0) {
        do {
            GETIOV(;);
            w = fp->_w;
            if (fp->_flags & 0x200) {
                if (len < w)
                    w = len;
                COPY(w);
                fp->_w -= w;
                fp->_p += w;
                w = len;
            } else if (fp->_p > fp->_bf._base && len > w) {
                COPY(w);
                fp->_p += w;
                if (fflush(fp))
                    goto err;
            } else if (len >= (w = fp->_bf._size)) {
                w = (*(s32 (*)(void *, const char *, s32))fp->_write)(fp->_cookie, p, w);
                if (w <= 0)
                    goto err;
            } else {
                w = len;
                COPY(w);
                fp->_w -= w;
                fp->_p += w;
            }
            p += w;
            len -= w;
        } while ((uio->uio_resid -= w) != 0);
    } else {
        nlknown = 0;
        do {
            GETIOV(nlknown = 0);
            if (!nlknown) {
                nl = memchr((void *)p, '\n', len);
                nldist = nl ? nl + 1 - p : len + 1;
                nlknown = 1;
            }
            s = MIN(len, nldist);
            w = fp->_w + fp->_bf._size;
            if (fp->_p > fp->_bf._base && s > w) {
                COPY(w);
                fp->_p += w;
                if (fflush(fp))
                    goto err;
            } else if (s >= (w = fp->_bf._size)) {
                w = (*(s32 (*)(void *, const char *, s32))fp->_write)(fp->_cookie, p, w);
                if (w <= 0)
                    goto err;
            } else {
                w = s;
                COPY(w);
                fp->_w -= w;
                fp->_p += w;
            }
            if ((nldist -= w) == 0) {
                if (fflush(fp))
                    goto err;
                nlknown = 0;
            }
            p += w;
            len -= w;
        } while ((uio->uio_resid -= w) != 0);
    }
    return 0;

err:
    fp->_flags |= 0x40;
    return -1;
}
