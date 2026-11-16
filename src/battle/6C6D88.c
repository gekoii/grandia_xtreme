#include "common.h"
#include "protos/battle.h"

typedef struct {
    u8 pad0[0x4];
    s32 off;
} Hdr;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x770 - 0x18];
} Ent;

typedef struct Obj {
    u8 pad0[0xC];
    struct Obj *unkC;
    u8 pad10[0x60 - 0x10];
    u8 unk60;
} Obj;

typedef struct {
    Ent *ent;
    Obj *unk4;
    Obj *unk8;
    u8 unkC;
    s8 unkD;
} Out;


void BtlPtclGenReqFill(Out *o, Hdr *h, s32 idx, Obj *obj) {
    Ent *e;

    e = (Ent *)((u8 *)h + h->off) + idx;
    o->ent = e;
    switch (*PtclPtbGetString(h, e->unk14)) {
    case 1:
        o->unkC = obj->unk60;
        o->unk4 = NULL;
        o->unk8 = NULL;
        break;
    case 2:
        o->unk8 = obj;
        o->unkC = 0;
        break;
    case 3:
        o->unk4 = obj;
        o->unkC = obj->unkC->unk60;
        o->unk8 = NULL;
        break;
    }
    o->unkD = 10;
}
