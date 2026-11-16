/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[0xC];
    u16 flags;
    s16 xE;
    u8 pad10[0x50 - 0x10];
    s32 x50;
    s32 x54;
} S8AED8;


s64 __sseek(S8AED8 *arg0, s32 arg1, s32 arg2) {
    s64 ret;

    ret = _lseek_r(arg0->x54, arg0->xE, arg1, arg2);
    if (ret == -1) {
        arg0->flags &= 0xEFFF;
    } else {
        arg0->x50 = ret;
        arg0->flags |= 0x1000;
    }
    return ret;
}
