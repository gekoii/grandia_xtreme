#include "common.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[4];
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} Sub;

void AmapOccSetChrMove(u8 *arg0, u16 arg1, f32 arg2, f32 arg3, f32 arg4) {
    Sub *s;

    *(f32 *)(arg0 + 0x20) = arg2;
    s = (Sub *)(arg0 + 0x20);
    *(s16 *)(arg0 + 0x18) = 0x14;
    s->unk4 = arg3;
    s->unk8 = -arg4;
    s->unk1A = 1;
    s->unk1C = 0;
    s->unk10 = 0;
    s->unk1E = 0;
    s->unk14 = 0;
    s->unk18 = 0;
    s->unk12 = arg1;
}
