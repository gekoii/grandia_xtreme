#include "common.h"
#include "protos/field.h"
#include "field/n22.h"


void FldPointLightAddFlicker(f32 *vec, s32 arg1, f32 f) {
    FldPointLight *e;
    FldLightRec *sub;

    e = &fldPointLights[fldPointLightCount];
    e->unk18 = f;
    e->range = f;
    e->unk5C = arg1;
    e->enable = 1;
    e->kind = 4;
    sub = &e->light;
    GphLightInit(sub);
    sub->type = 1;
    sub->color[0] = vec[0];
    sub->color[1] = vec[1];
    sub->color[2] = vec[2];
    sub->color[3] = vec[3];
    sub->range = f;
    sub->intensity = 6.0f;
    sub->enable = 1;
    fldPointLightCount++;
}
