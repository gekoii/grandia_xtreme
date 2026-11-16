/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef char *va_list;
#define va_start(ap, last) \
    (ap = ((va_list)__builtin_next_arg(last) - \
           (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))


void _sceSioPrintf(s32 arg0, ...) {
    va_list ap;

    va_start(ap, arg0);
    _sceVprintf(arg0, ap);
}
