/* Matches newlib's __smakebuf (makebuf.c). Third-party library code: see README.md, "Third-party library code". */
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
    u8 pad0[0x3C];
    void (*__cleanup)(struct _reent *);
};

struct stat {
    s32 st_dev;
    u32 st_mode;
    u8 pad8[0x60];
};

extern void *_malloc_r(struct _reent *, u32);
extern s32 __sseek();

void __smakebuf(register FILE *fp) {
    register u32 size, couldbetty;
    register void *p;
    struct stat st;

    if (fp->_flags & 0x2) {
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
    if (fp->_file < 0 || _fstat_r(fp->_data, fp->_file, &st) < 0) {
        couldbetty = 0;
        size = 1024;
        fp->_flags |= 0x800;
    } else {
        couldbetty = (st.st_mode & 0170000) == 0020000;
        size = 1024;
        if ((st.st_mode & 0170000) == 0100000 && fp->_seek == __sseek) {
            fp->_flags |= 0x400;
            fp->_blksize = 1024;
        } else
            fp->_flags |= 0x800;
    }
    if ((p = _malloc_r(fp->_data, size)) == NULL) {
        fp->_flags |= 0x2;
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
    } else {
        fp->_data->__cleanup = _cleanup_r;
        fp->_flags |= 0x80;
        fp->_bf._base = fp->_p = (u8 *)p;
        fp->_bf._size = size;
        if (couldbetty && isatty(fp->_file))
            fp->_flags |= 0x1;
    }
}
