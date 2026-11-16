/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} S;

S *Balloc(s32, s32);

S *i2b(s32 arg0, s32 arg1) {
    S *p = Balloc(arg0, 1);
    p->unk14 = arg1;
    p->unk10 = 1;
    return p;
}
