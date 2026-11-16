#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    s8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    s8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} Sub;

void AmapOccSetChrFlags(u8 *arg0, u16 arg1, s8 arg2, s8 arg3) {
    Sub *s;

    s = (Sub *)(arg0 + 0x20);
    *(s16 *)(arg0 + 0x18) = 0x23;
    *(s16 *)(arg0 + 0x20) = 0;
    s->unk2 = arg1;
    s->unk5 = arg2;
    s->unkA = arg3;
    s->unk4 = 0;
    s->unk6 = 0;
    s->unk7 = 0;
    s->unk8 = 0;
    s->unk9 = 0;
    s->unkB = 0;
    s->unkC = 0;
    s->unkD = 0;
    s->unkE = 0;
}
