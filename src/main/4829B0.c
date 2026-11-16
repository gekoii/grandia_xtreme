/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 *p;
    s32 r;
    s32 w;
    s16 flags;
    s16 file;
    u8 *base;
    s32 size;
    u8 pad18[0x54 - 0x18];
    s32 data;
    u8 pad58[0x60 - 0x58];
} File;


s32 _sprintf_r(s32 arg0, u8 *buf, const char *fmt, ...) {
    File f;
    s32 ret;
    void *ap;

    f.data = arg0;
    f.flags = 0x208;
    f.p = buf;
    f.w = 0x7FFFFFFF;
    f.base = buf;
    f.size = 0x7FFFFFFF;
    ap = (u8 *)__builtin_next_arg(fmt) - 5 * 8; /* back up to the 5 register-saved vararg slots */
    ret = vfprintf(&f, fmt, ap);
    *f.p = 0;
    return ret;
}
