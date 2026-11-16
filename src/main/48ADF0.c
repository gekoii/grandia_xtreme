/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
typedef struct { u8 pad[0xC]; u16 flags; s16 id; u8 pad10[0x40]; s32 pos; s32 handle; } File;
s32 __sread(File *f, s32 arg1, s32 arg2) {
    s32 n = _read_r(f->handle, f->id, arg1, arg2);
    if (n >= 0) {
        f->pos += n;
    } else {
        f->flags &= 0xEFFF;
    }
    return n;
}
