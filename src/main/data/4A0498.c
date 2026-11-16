/* Third-party library code (newlib): see README.md, "Third-party library code". */
/* main executable: newlib's reentrancy data (impure.c: impure_data and
 * _impure_ptr) and locale.c's __mb_cur_max and LC_CTYPE names. Data-only C
 * (plan step 6b, docs/shifting.md); the users are __errno (482760.c), the
 * stdio code (__sinit, printf, fflush, ...) and _vfprintf_r (484690.c).
 * The layout is the old (1.8 era) newlib's, as in include/sdk/newlib.h.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x004A0498-0x004A07A0 */

typedef unsigned char u8;

/* impure.c's "C" (_current_locale), in its .rodata (main data/4A8448) */
extern char __impureLocaleC[];

struct _reent;

struct __sbuf {
    unsigned char *_base;
    int _size;
};

/* FILE (0x58) */
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
    int (*_seek)(void *, int, int);
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

struct _glue {
    struct _glue *_next;
    int _niobs;
    FILE *_iobs;
};

struct _atexit {
    struct _atexit *_next;
    int _ind;
    void (*_fns[32])(void);
};

struct __tm {
    int __tm_sec;
    int __tm_min;
    int __tm_hour;
    int __tm_mday;
    int __tm_mon;
    int __tm_year;
    int __tm_wday;
    int __tm_yday;
    int __tm_isdst;
};

struct _Bigint;

/* struct _reent (0x2EC) */
struct _reent {
    int _errno;
    FILE *_stdin;
    FILE *_stdout;
    FILE *_stderr;
    int _inc;
    char _emergency[25];
    int _current_category;
    const char *_current_locale;
    int __sdidinit;
    void (*__cleanup)(struct _reent *);
    struct _Bigint *_result;
    int _result_k;
    struct _Bigint *_p5s;
    struct _Bigint **_freelist;
    int _cvtlen;
    char *_cvtbuf;
    union {
        struct {
            unsigned int _rand_next;
            char *_strtok_last;
            char _asctime_buf[26];
            struct __tm _localtime_buf;
            int _gamma_signgam;
        } _reent;
        struct {
            unsigned char *_nextf[30];
            unsigned int _nmalloc[30];
        } _unused;
    } _new;
    struct _atexit *_atexit;
    struct _atexit _atexit0;
    void (**_sig_func)(int);
    struct _glue __sglue;
    FILE __sf[3];
};

/* newlib's static impure_data = _REENT_INIT(impure_data).
 * fields: D_004A067C __sf[0], D_004A06D4 __sf[1], D_004A072C __sf[2] */
struct _reent impure_data = {
    0, &impure_data.__sf[0], &impure_data.__sf[1], &impure_data.__sf[2],
    0, "", 0, __impureLocaleC, 0, 0, 0, 0, 0, 0, 0, 0,
    { { 1 } }, /* _rand_next */
};

struct _reent *_impure_ptr = &impure_data;

/* locale.c */
int __mb_cur_max = 1;
static char lc_ctype[8] = "C";
static char last_lc_ctype[8] = "C";
