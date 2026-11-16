#include "common.h"

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
} Rect5;

void EvtMsgLayoutSet(Rect5 *, s32);

void EvtMsgFrameLayoutInit(void) {
    Rect5 r;

    r.unk0 = 0;
    r.unk2 = 1;
    r.unk4 = 0x15;
    r.unk6 = 0x26;
    r.unk8 = 0x1B;
    EvtMsgLayoutSet(&r, 0);
    r.unk0 = 1;
    r.unk2 = 1;
    r.unk4 = 3;
    r.unk6 = 0x26;
    r.unk8 = 9;
    EvtMsgLayoutSet(&r, 1);
    r.unk0 = 1;
    r.unk2 = 1;
    r.unk4 = 3;
    r.unk6 = 0x26;
    r.unk8 = 9;
    EvtMsgLayoutSet(&r, 2);
    r.unk0 = 1;
    r.unk2 = 0x10;
    r.unk4 = 0x14;
    r.unk6 = 0x24;
    r.unk8 = 0x18;
    EvtMsgLayoutSet(&r, 3);
    r.unk0 = 1;
    r.unk2 = 0x13;
    r.unk4 = 1;
    r.unk6 = 0x14;
    r.unk8 = 3;
    EvtMsgLayoutSet(&r, 4);
    r.unk0 = 1;
    r.unk2 = 0x13;
    r.unk4 = 1;
    r.unk6 = 0x14;
    r.unk8 = 3;
    EvtMsgLayoutSet(&r, 5);
    r.unk0 = 1;
    r.unk2 = 0x26;
    r.unk4 = 8;
    r.unk6 = 0x27;
    r.unk8 = 3;
    EvtMsgLayoutSet(&r, 6);
}
