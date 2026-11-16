/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 *p;
    s32 r;
    s32 w;
    u16 flags;
    s16 file;
    u8 *base;
    s32 size;
    s32 lbfsize;
    s32 cookie;
    s32 read;
    s32 write;
    u8 pad28[0x54 - 0x28];
    s32 data;
    u8 pad58[0x60 - 0x58];
} File;

s32 fflush(File *);

s32 __sbprintf(File *fp, const char *fmt, void *ap) {
    s32 ret;
    File fake;
    u8 buf[0x400];

    fake.data = fp->data;
    fake.flags = fp->flags & ~2;
    fake.file = fp->file;
    fake.cookie = fp->cookie;
    fake.write = fp->write;
    fake.base = fake.p = buf;
    fake.size = fake.w = sizeof(buf);
    fake.lbfsize = 0;
    ret = vfprintf(&fake, fmt, ap);
    if (ret >= 0 && fflush(&fake)) {
        ret = -1;
    }
    if (fake.flags & 0x40) {
        fp->flags |= 0x40;
    }
    return ret;
}
