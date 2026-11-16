/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    volatile u32 flags;
    u8 pad4[0xC];
    u32 unk10;
    u8 pad14[0xC];
    s32 unk20;
} Chan;

extern u8 _sceDmaSyncTimeoutStr[];

void sceDmaRecvI(Chan *ch, s32 arg1, s32 arg2) {
    s32 n;
    s32 v;
    s32 w;

    n = 0x1000000;
    while (ch->flags & 0x100) {
        if (--n < 0) {
            printf(_sceDmaSyncTimeoutStr);
            v = ch->flags;
            if (((u32)v >> 8) & 1) {
                do {
                    w = v & ~0x100;
                    v = w;
                } while (((u32)v >> 8) & 1);
                ch->flags = w;
            }
        }
    }
    if (ch->unk10 != 0xFFFFFFFF) {
        ch->unk10 = arg1;
    }
    ch->unk20 = arg2;
    ch->flags = (((s32)ch->flags & ~0xC | 8) & ~1) | 0x100;
}
