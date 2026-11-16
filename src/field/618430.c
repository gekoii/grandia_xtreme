#include "common.h"
#include "protos/field.h"
#include "field/n22.h"


void FldPointLightAdd(f32 *pos, f32 *vec, f32 f) {
    FldPointLight *e;
    FldLightRec *sub;

    e = &fldPointLights[fldPointLightCount];
    e->enable = 1;
    sub = &e->light;
    GphLightInit(sub);
    sub->pos[0] = pos[0];
    sub->pos[1] = pos[1];
    sub->pos[2] = pos[2];
    sub->type = 1;
    sub->pos[3] = 1.0f;
    sub->color[0] = vec[0];
    sub->color[1] = vec[1];
    sub->color[2] = vec[2];
    sub->color[3] = vec[3];
    sub->range = f;
    sub->intensity = 5.0f;
    sub->enable = 1;
    fldPointLightCount++;
}
