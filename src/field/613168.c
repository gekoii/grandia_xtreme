#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Sub;

void AmapOccSetChrFlagB(u8 *arg0, s16 arg1, s16 arg2, s16 arg3) {
    Sub *s;

    s = (Sub *)(arg0 + 0x20);
    *(s16 *)(arg0 + 0x18) = 0x12;
    s->unk2 = arg1;
    s->unk4 = arg2;
    s->unk8 = 2;
    s->unk6 = arg3;
    *(s16 *)(arg0 + 0x20) = 0;
}
