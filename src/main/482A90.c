/* Matches newlib's sscanf (sscanf.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef char *va_list;
#define va_start(AP, LAST)                                                         \
    (AP = ((va_list)__builtin_next_arg(LAST) -                                     \
           (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0)))

struct _reent;

struct __sbuf {
    unsigned char *_base;
    int _size;
};

typedef struct __sFILE {
    unsigned char *_p;
    int _r;
    int _w;
    short _flags;
    short _file;
    struct __sbuf _bf;
    int _lbfsize;
    void *_cookie;
    int (*_read)(void *, char *, int);
    int (*_write)(void *, const char *, int);
    s32 (*_seek)(void *, s32, int);
    int (*_close)(void *);
    struct __sbuf _ub;
    unsigned char *_up;
    int _ur;
    unsigned char _ubuf[3];
    unsigned char _nbuf[1];
    struct __sbuf _lb;
    int _blksize;
    int _offset;
    struct _reent *_data;
} FILE;

#define __SRD 0x0004
#define __SWR 0x0008
#define __SSTR 0x0200
#define INT_MAX 0x7fffffff

extern struct _reent *_impure_ptr;

s32 strlen(const char *);
s32 __svfscanf(FILE *, const char *, va_list);

s32 sscanf(const char *str, const char *fmt, ...) {
    s32 ret;
    va_list ap;
    FILE f;

    f._flags = __SRD;
    f._bf._base = f._p = (unsigned char *)str;
    f._bf._size = f._r = strlen(str);
    f._read = eofread;
    f._ub._base = NULL;
    f._lb._base = NULL;
    f._data = _impure_ptr;
    va_start(ap, fmt);
    ret = __svfscanf(&f, fmt, ap);
    return ret;
}
