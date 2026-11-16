#include "common.h"
#include "protos/field.h"
#include "field/n22.h"



void FldPointLightLoad(FldPointLightDef *src, s32 n) {
    FldPointLight *base;
    FldPointLightDef *s;
    FldPointLight *e;
    FldLightRec *sub;
    s32 i;

    if (n == 0) {
        return;
    }
    base = &fldPointLights[fldPointLightCount];
    for (i = 0; i < n; i++) {
        if ((u32)(fldPointLightCount + n) >= 32) {
            n = 0;
            fldPointLightCount = 32;
            break;
        }
        s = &src[i];
        if (s->enable) {
            base[i].enable = 1;
        } else {
            base[i].enable = 0;
        }
        e = &base[i];
        e->id = s->id;
        e->kind = s->attachKind;
        switch (s->attachKind) {
        case 0:
            e->attach = 0;
            break;
        case 1:
            e->attach = FldObjGet(s->attachId);
            break;
        case 2:
            e->attach = ChrTenantGet(s->attachId);
            break;
        case 3:
            e->attach = ChrGetById(s->attachId);
            break;
        }
        base[i].unk6 = s->unk5;
        sub = &base[i].light;
        GphLightInit(sub);
        sub->pos[0] = s->pos[0];
        sub->type = 1;
        sub->enable = 1;
        sub->pos[1] = s->pos[1];
        sub->pos[2] = s->pos[2];
        sub->pos[3] = 1.0f;
        sub->color[0] = s->color[0];
        sub->color[1] = s->color[1];
        sub->color[2] = s->color[2];
        sub->color[3] = 0.0f;
        sub->intensity = s->intensity;
        sub->range = s->range;
    }
    fldPointLightCount += n;
}
