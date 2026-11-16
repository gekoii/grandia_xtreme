#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void AmapBuildSoundEmitters(AmapWork *o) {
    s32 i;
    AmapPlace *r;
    FldSndEmitterDef *arr;
    FldSndEmitterDef *e;
    s32 x;
    s32 z;

    i = 0;
    o->sndEmitterCount = 0;
    r = o->gimmicks;
    arr = o->sndEmitters;
    for (; i < o->gimmickCount; i++) {
        {
            if ((r[i].type & 0x1F) == 0x1C) {
                z = r[i].y * 500;
                x = r[i].x * 500;
                e = &arr[o->sndEmitterCount];
                e->id = o->sndEmitterCount;
                e->se = VuRandInt(4) + 3000;
                e->pos[0] = x;
                e->pos[2] = -z;
                e->attachKind = 0;
                e->attachId = 0;
                e->shape = 0;
                e->unk7 = 5;
                e->pos[1] = 0;
                e->unkE = 0;
                e->pos2[0] = 0;
                e->pos2[1] = 0;
                e->pos2[2] = 0;
                e->unk16 = 500;
                e->unk18[0] = 0;
                e->unk18[1] = 0;
                e->unk18[2] = 0;
                e->unk18[3] = 0;
                o->sndEmitterCount++;
            }
        }
    }
}
