#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} Sub;

void AmapOccSetMotion(u8 *arg0, u16 arg1) {
    Sub *s;

    *(s16 *)(arg0 + 0x18) = 7;
    s = (Sub *)(arg0 + 0x20);
    *(s16 *)(arg0 + 0x20) = 1;
    s->unk2 = arg1;
    s->unk6 = 3;
    s->unk8 = 1;
    s->unk4 = 0;
    s->unkA = 0;
}
