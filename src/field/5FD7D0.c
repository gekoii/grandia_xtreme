#include "common.h"
#include "protos/field.h"

typedef char *va_list;
#define va_start(AP, LAST)                                                         \
    (AP = ((va_list)__builtin_next_arg(LAST) -                                     \
           (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0)))


void EvtDbgPrintf(s32 x, s32 y, char *fmt, ...) {
    u8 buf[0x200];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
}
