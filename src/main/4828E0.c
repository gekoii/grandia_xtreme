/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef char *va_list;
#define va_start(AP, LAST)                                                         \
    (AP = ((va_list)__builtin_next_arg(LAST) -                                     \
           (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0)))

typedef struct {
    u8 pad0[8];
    s32 w8;
} Obj;

void _vfprintf_r(Obj *, s32, s32, va_list);

void _printf_r(Obj *arg0, s32 arg1, ...) {
    va_list ap;
    va_start(ap, arg1);
    _vfprintf_r(arg0, arg0->w8, arg1, ap);
}
