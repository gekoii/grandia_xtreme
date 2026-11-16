#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void FldSndEmitterLoad(FldSndEmitterDef *src, s32 n) {
    s32 i;
    FldSndEmitterDef *s;
    FldSndEmitter *d;
    f32 cx, cy, cz;

    fldSndEmitterCount = n;
    for (i = 0; i < n; i++) {
        s = &src[i];
        d = &fldSndEmitters[i];
        d->id = s->id;
        d->se = s->se;
        d->slot = -1;
        d->state = 0;
        d->timer = 0;
        d->shape = s->shape;
        d->flags = s->unk7;
        switch (d->shape) {
        case 1:
            cx = (f32)(s->pos[0] + s->pos2[0]) * 0.5f;
            d->pos[0] = cx;
            cy = (f32)(s->pos[1] + s->pos2[1]) * 0.5f;
            d->pos[1] = cy;
            cz = (f32)(s->pos[2] + s->pos2[2]) * 0.5f;
            d->pos[2] = cz;
            d->boxMin[0].f = (f32)s->pos[0] - cx;
            d->boxMin[1].f = (f32)s->pos[1] - cy;
            d->boxMin[2].f = (f32)s->pos[2] - cz;
            d->boxMax[0].f = (f32)s->pos2[0] - cx;
            d->boxMax[1].f = (f32)s->pos2[1] - cy;
            d->boxMax[2].f = (f32)s->pos2[2] - cz;
            break;
        case 2:
            d->pos[0] = s->pos[0];
            d->pos[1] = s->pos[1];
            d->pos[2] = s->pos[2];
            d->boxMax[0].f = s->pos2[2];
            break;
        case 0:
        default:
            d->pos[0] = s->pos[0];
            d->pos[1] = s->pos[1];
            d->pos[2] = s->pos[2];
            break;
        }
        switch (s->attachKind) {
        case 0:
            d->attach = 0;
            break;
        case 1:
            d->attach = FldObjGet(s->attachId);
            break;
        case 2:
            d->attach = (s32)ChrTenantGet(s->attachId)->model;
            break;
        }
        d->unk28 = 0xFF;
        d->unk24 = s->unkE;
        d->period = s->unk16;
        d->unk34 = 0;
        d->unk38[0] = s->unk18[0];
        d->unk38[1] = s->unk18[1];
        d->unk38[2] = s->unk18[2];
        d->unk38[3] = s->unk18[3];
    }
}
