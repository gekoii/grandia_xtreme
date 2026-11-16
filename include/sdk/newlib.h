/*
 * newlib's reentrancy structure, stdio FILE and malloc state, with newlib's
 * own names.
 *
 * The game links an old newlib (the 1.8 era): struct _reent's _new union
 * starts with a 32-bit _rand_next (rand at 482970 uses _impure_ptr+0x58 as an
 * int) and FILE ends with its _data back pointer. sizeof(struct _reent) is
 * 0x2EC, the size of impure_data (0x004A0498) up to _impure_ptr.
 */
#ifndef SDK_NEWLIB_H
#define SDK_NEWLIB_H

#include "common.h"

struct _reent;

/* struct __sbuf (8). */
struct __sbuf {
    /* 0x00 */ unsigned char *_base;
    /* 0x04 */ int _size;
};

/* FILE (0x58). */
typedef struct __sFILE {
    /* 0x00 */ unsigned char *_p;
    /* 0x04 */ int _r;
    /* 0x08 */ int _w;
    /* 0x0C */ short _flags;
    /* 0x0E */ short _file;
    /* 0x10 */ struct __sbuf _bf;
    /* 0x18 */ int _lbfsize;
    /* 0x1C */ void *_cookie;
    /* 0x20 */ int (*_read)(void *, char *, int);
    /* 0x24 */ int (*_write)(void *, const char *, int);
    /* 0x28 */ s32 (*_seek)(void *, s32, int);
    /* 0x2C */ int (*_close)(void *);
    /* 0x30 */ struct __sbuf _ub;
    /* 0x38 */ unsigned char *_up;
    /* 0x3C */ int _ur;
    /* 0x40 */ unsigned char _ubuf[3];
    /* 0x43 */ unsigned char _nbuf[1];
    /* 0x44 */ struct __sbuf _lb;
    /* 0x4C */ int _blksize;
    /* 0x50 */ int _offset;
    /* 0x54 */ struct _reent *_data;
} FILE;

/* struct _glue (0xC): a block of FILEs. */
struct _glue {
    /* 0x00 */ struct _glue *_next;
    /* 0x04 */ int _niobs;
    /* 0x08 */ FILE *_iobs;
};

/* struct _atexit (0x88). */
struct _atexit {
    /* 0x00 */ struct _atexit *_next;
    /* 0x04 */ int _ind;
    /* 0x08 */ void (*_fns[32])(void);
};

/* struct tm (0x24), _localtime_buf. */
struct __tm {
    /* 0x00 */ int __tm_sec;
    /* 0x04 */ int __tm_min;
    /* 0x08 */ int __tm_hour;
    /* 0x0C */ int __tm_mday;
    /* 0x10 */ int __tm_mon;
    /* 0x14 */ int __tm_year;
    /* 0x18 */ int __tm_wday;
    /* 0x1C */ int __tm_yday;
    /* 0x20 */ int __tm_isdst;
};

struct _Bigint;

#define _REENT_EMERGENCY_SIZE 25
#define _REENT_ASCTIME_SIZE 26
#define _N_LISTS 30

/* struct _reent (0x2EC). */
struct _reent {
    /* 0x000 */ int _errno;
    /* 0x004 */ FILE *_stdin;
    /* 0x008 */ FILE *_stdout;
    /* 0x00C */ FILE *_stderr;
    /* 0x010 */ int _inc;
    /* 0x014 */ char _emergency[_REENT_EMERGENCY_SIZE];
    /* 0x030 */ int _current_category;
    /* 0x034 */ const char *_current_locale;
    /* 0x038 */ int __sdidinit;
    /* 0x03C */ void (*__cleanup)(struct _reent *);
    /* 0x040 */ struct _Bigint *_result;
    /* 0x044 */ int _result_k;
    /* 0x048 */ struct _Bigint *_p5s;
    /* 0x04C */ struct _Bigint **_freelist;
    /* 0x050 */ int _cvtlen;
    /* 0x054 */ char *_cvtbuf;
    /* 0x058 */ union {
        struct {
            /* 0x058 */ unsigned int _rand_next;
            /* 0x05C */ char *_strtok_last;
            /* 0x060 */ char _asctime_buf[_REENT_ASCTIME_SIZE];
            /* 0x07C */ struct __tm _localtime_buf;
            /* 0x0A0 */ int _gamma_signgam;
        } _reent;
        struct {
            /* 0x058 */ unsigned char *_nextf[_N_LISTS];
            /* 0x0D0 */ unsigned int _nmalloc[_N_LISTS];
        } _unused;
    } _new;
    /* 0x148 */ struct _atexit *_atexit;
    /* 0x14C */ struct _atexit _atexit0;
    /* 0x1D4 */ void (**_sig_func)(int);
    /* 0x1D8 */ struct _glue __sglue;
    /* 0x1E4 */ FILE __sf[3];
};

extern struct _reent *_impure_ptr;
extern int __mb_cur_max;

/* ---- malloc (mallocr.c) ---- */

/* struct mallinfo (0x28); __malloc_current_mallinfo.arena is sbrked_mem. */
struct mallinfo {
    /* 0x00 */ int arena;
    /* 0x04 */ int ordblks;
    /* 0x08 */ int smblks;
    /* 0x0C */ int hblks;
    /* 0x10 */ int hblkhd;
    /* 0x14 */ int usmblks;
    /* 0x18 */ int fsmblks;
    /* 0x1C */ int uordblks;
    /* 0x20 */ int fordblks;
    /* 0x24 */ int keepcost;
};

/* struct malloc_chunk (0x10), INTERNAL_SIZE_T = 32 bits. */
struct malloc_chunk {
    /* 0x00 */ u32 prev_size;
    /* 0x04 */ u32 size;
    /* 0x08 */ struct malloc_chunk *fd;
    /* 0x0C */ struct malloc_chunk *bk;
};

/* av_: 128 bins as {fd, bk} pairs plus the two leading entries (top is
 * av_[2]). */
extern struct malloc_chunk *__malloc_av_[128 * 2 + 2];
/* unsigned long is 64-bit on the EE. */
extern unsigned long __malloc_trim_threshold;
extern unsigned long __malloc_top_pad;
extern char *__malloc_sbrk_base;
extern unsigned long __malloc_max_sbrked_mem;
extern unsigned long __malloc_max_total_mem;
extern struct mallinfo __malloc_current_mallinfo;

/* newlib's math wrappers' error mode (_LIB_VERSION_TYPE, 1 = _XOPEN_).
 * const: powf (0047BAB0) only matches with it, and the other wrappers match
 * either way (newlib itself does not declare it const). */
extern const s32 _LIB_VERSION;
/* The VIEWER overlay links its own copy (0x007034CC). */
extern const s32 _LIB_VERSION_vwr;

#endif
