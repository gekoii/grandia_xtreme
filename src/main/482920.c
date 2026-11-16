/* Matches newlib's printf (printf.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef char *va_list;
#define va_start(AP, LAST)                                                         \
    (AP = ((va_list)__builtin_next_arg(LAST) -                                     \
           (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0)))

struct _reent;
typedef struct {
    u8 pad0[0x54];
    struct _reent *_data;
} FILE;

struct _reent {
    u8 pad0[8];
    FILE *_stdout;
};

extern struct _reent *_impure_ptr;

s32 printf(const char *fmt, ...) {
    s32 ret;
    va_list ap;

    _impure_ptr->_stdout->_data = _impure_ptr;
    va_start(ap, fmt);
    ret = vfprintf(_impure_ptr->_stdout, fmt, ap);
    return ret;
}
