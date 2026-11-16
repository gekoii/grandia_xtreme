/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "battle/n30.h"
#include "sys.h"



void FxDrainOrbDraw(FxAtk *e) {
    FxDrainSpark *sp;
    FxDrainOrb *fx;
    VuVec3 v;
    s32 i;
    f32 t;
    f32 sz;

    fx = e->work;
    GphTexBind(0x36);
    SprBegin(0.0f, 0.0f);
    sp = fx->sparks;
    sprState.blend = 2;
    for (i = 23; i >= 0; i--, sp++) {
        if (sp->life <= 0) {
            continue;
        }
        v.x = sp->pos[0];
        v.y = sp->pos[1];
        v.z = sp->pos[2];
        if (sp->age < 5) {
            sprState.sx = sp->age * 0.8f;
        } else if (sp->age < 20) {
            sprState.sx = 4.0f - (sp->age - 5) * 0.13333334f;
        } else {
            sprState.sx = 2.0f - 2.0f / (sp->life - 20) * (sp->age - 20);
        }
        sz = sprState.sx * sp->size;
        sprState.sy = sz;
        sprState.sx = sz;
        if (sp->age < 10) {
            t = sp->age * 0.1f;
            sprState.col[0].r = -64.0f * t + 128.0f;
            sprState.col[0].g = -127.0f * t + 255.0f;
            sprState.col[0].b = -127.0f * t + 255.0f;
        } else if (sp->age < 20) {
            t = (sp->age - 10) * 0.1f;
            sprState.col[0].g = -58.0f * t + 128.0f;
            sprState.col[0].r = -64.0f * t + 64.0f;
            sprState.col[0].b = -28.0f * t + 128.0f;
        } else {
            t = 1.0f / (sp->life - 20) * (sp->age - 20);
            sprState.col[0].r = 18.0f * t + 0.0f;
            sprState.col[0].g = -70.0f * t + 70.0f;
            sprState.col[0].b = -36.0f * t + 100.0f;
        }
        SprDrawRect3D(&ptclGlowRect, -1, -1, &v);
    }
    SprFlush();
}
