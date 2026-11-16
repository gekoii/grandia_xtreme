#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
} Sub;

void AmapOccSetScale(u8 *arg0, u16 arg1, s16 arg2, s16 arg3) {
    Sub *s;

    *(s16 *)(arg0 + 0x18) = 4;
    s = (Sub *)(arg0 + 0x20);
    *(u16 *)(arg0 + 0x16) |= 0x300;
    *(s16 *)(arg0 + 0x20) = 0;
    s->unk2 = 1;
    s->unk6 = 1;
    s->unk8 = 1;
    s->unk4 = arg1;
    s->unkA = 10;
    s->unkC = 0;
    s->unk18 = 1;
    s->unk1C = 1.0f;
    s->unk10 = 0;
    s->unk14 = 0;
    s->unk20 = arg2;
    s->unk22 = arg3;
}
