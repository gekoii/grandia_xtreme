#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
} Sub;

void AmapOccSetTrap(u8 *arg0, s16 arg1, s16 arg2) {
    Sub *s;

    *(s16 *)(arg0 + 0x20) = arg1;
    *(s16 *)(arg0 + 0x18) = 12;
    s = (Sub *)(arg0 + 0x20);
    s->unk2 = 0;
    s->unk4 = 0;
    s->unk6 = 0;
    s->unk8 = 0;
    s->unkC = 0;
    s->unk12 = arg2;
}
