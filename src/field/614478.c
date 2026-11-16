#include "common.h"
#include "field/n22.h"

void AmapBuildPointLights(AmapWork *o) {
    s32 i;
    s32 t;
    s32 f;
    AmapObjRec *recs;
    FldPointLightDef *l;

    i = 0;
    recs = o->objs;
    l = o->lights;
    if (i >= o->objCount || o->lightCount >= 16) {
        return;
    }
    /* goto loop: entered at body or at the type-0x10 map-type check, which
     * the original places at the loop's bottom; structured loops reorder the blocks */
    if ((recs[i].type & 0x1F) != 0x10) {
        goto body;
    }
    goto check;
    for (;;) {
        if (f == 7) {
            return;
        }
    body:
        t = recs[i].type & 0x1F;
        if (t == 0x11) {
            if (o->mapType == 2 || o->mapType == 4 || o->mapType == 5 || o->mapType == 6 || o->mapType == 7) {
                return;
            }
        }
        if (t < 0x12) {
            if (t >= 0x10) {
                l->id = o->lightCount + 1;
                l->enable = 1;
                l->unk2 = 0;
                l->attachKind = 0;
                l->attachId = 0;
                l->unk5 = 0;
                l->unk7 = 0;
                l->pos[0] = recs[i].x;
                l->pos[1] = 350.0f;
                l->pos[2] = -recs[i].z;
                l->intensity = 32.0f;
                l->unk14 = 0.0f;
                l->unk18 = 0.0f;
                l->unk20 = 0.0f;
                l->color[0] = 255.0f;
                l->color[1] = 255.0f;
                l->color[2] = 255.0f;
                l->range = 400.0f;
                l->unk30 = 0.0f;
                l->unk34 = 0.0f;
                l->unk3C = 0.0f;
                o->lightCount++;
                l++;
            }
        }
        i++;
        if (i >= o->objCount || o->lightCount >= 16) {
            return;
        }
        if ((recs[i].type & 0x1F) != 0x10) {
            goto body;
        }
    check:
        f = o->mapType;
        if ((u8)f == 2 || (u8)f == 6) {
            return;
        }
    }
}
