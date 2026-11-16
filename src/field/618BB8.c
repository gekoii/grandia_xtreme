#include "common.h"
#include "field/n22.h"

extern void GphSphereDraw(s32, f32 *, u32);

void FldPointLightDebugDraw(void) {
    f32 r[3];
    f32 sph[4];
    u32 i;
    FldPointLight *e;
    FldLightRec *sub;
    u16 flag;

    r[0] = 100.0f;
    r[1] = 100.0f;
    r[2] = 100.0f;
    for (i = 0; i < fldPointLightCount; i++) {
        e = &fldPointLights[i];
        sub = &e->light;
        flag = e->enable;
        r[0] = sub->range;
        r[1] = sub->range;
        r[2] = sub->range;
        sph[0] = sub->pos[0];
        sph[1] = sub->pos[1];
        sph[2] = sub->pos[2];
        sph[3] = r[0];
        if (flag == 1) {
            GphSphereDraw(1, sph, 0x4040FF40);
        } else {
            GphSphereDraw(1, sph, 0x4080FF80);
        }
    }
}
