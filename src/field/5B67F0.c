#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct Sub {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
} Sub;

typedef struct Obj {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 pad4[0x12];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ u8 pad18[0x8];
    /* 0x20 */ Sub sub;
} Obj;

typedef struct Model {
    u8 pad[0x10];
    FVECTOR pos;
} Model;

typedef struct Ent {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ s32 unk54;
    /* 0x058 */ u8 pad58[0x8];
    /* 0x060 */ Model *model;
    /* 0x064 */ u8 pad64[0x11C];
    /* 0x180 */ FVECTOR unk180;
} Ent;


void OccScriptForward(Obj *obj)
{
    Sub *s = &obj->sub;
    Ent *e;
    FVECTOR v;

    if (EvtScnrIsRunning() != 1) {
        EvtScnrCall(s->unk0);
        if (obj->unk16 & 0x100) {
            e = ChrGetByNo(1);
            v[0] = s->unk4;
            v[1] = s->unk8;
            v[2] = -s->unkC;
            v[3] = 0.0f;
            e->unk54 = (FldCalcYaw(e->model->pos, v) - 0x8000) & 0xFFFF;
            __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(e->unk180) : "memory");
        }
        obj->unk2 = 0;
        obj->unk16 &= 0xFFCF;
    }
}
