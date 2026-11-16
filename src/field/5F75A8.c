/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"

typedef struct {
    u16 x0;
    u8 pad2[2];
    u16 x4;
    u8 pad6[0x10 - 6];
    f32 x10;
    s32 x14;
    f32 ang[2];
    f32 f20[7];
    s32 x3C[3];
    u8 a[10];
    u8 b[5];
} Src;


void FldCamConfigLoad(Src *src) {
    FldCamConfig *o;
    s32 i;

    o = FldCamConfigGet();
    o->type = src->x0;
    o->unk4 = src->x14;
    o->unkA = src->x4;
    o->yaw = src->x10;
    camCtl.unk5 = o->type;
    switch (camCtl.unk5) {
    case 0:
    case 1:
        camCtl.prevType = 5;
        break;
    case 2:
    case 3:
    case 5:
        camCtl.prevType = 0;
        break;
    }
    camCtl.aiType = camCtl.prevType;
    for (i = 0; i < 2; i++) {
        o->ang[i] = src->ang[i];
        while (o->ang[i] < -180.0f) {
            o->ang[i] += 360.0f;
        }
        while (o->ang[i] > 180.0f) {
            o->ang[i] -= 360.0f;
        }
    }
    o->unk18[0] = src->f20[0];
    o->aux = fldCamConfigAux;
    o->unk18[1] = src->f20[1];
    o->unk18[2] = src->f20[2];
    o->unk18[3] = src->f20[3];
    o->unk18[4] = src->f20[4];
    o->unk18[5] = src->f20[5];
    o->unk18[6] = src->f20[6];
    for (i = 0; i < 3; i++) {
        o->unk34[i] = src->x3C[i];
    }
    for (i = 0; i < 5; i++) {
        o->unk40[i][0] = src->a[i];
        o->unk40[i][1] = src->b[i];
    }
    camField[0].yaw = o->yaw + 1.570796f;
}
