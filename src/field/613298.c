#include "common.h"

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 unk11;
    s8 unk12;
    u8 pad13;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Sub;

void AmapOccSetMapEffect(u8 *arg0, s16 arg1, s16 arg2, s8 arg3) {
    Sub *s;

    *(s16 *)(arg0 + 0x18) = 0x28;
    s = (Sub *)(arg0 + 0x20);
    *(u16 *)(arg0 + 0x16) |= 0x100;
    s->unk16 = arg2;
    s->unk12 = arg3;
    s->unk14 = arg1;
    s->unk11 = 0;
    s->unk18 = 0;
    s->unk1A = 0;
    s->unk10 = 0;
}
