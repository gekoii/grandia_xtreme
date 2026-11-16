#include "common.h"
#include "gph.h"
#include "field/n19.h"
#include "protos/field.h"
#include "field/state.h"

void VuDirFromAngles(f32 *, f32 *);

void FldLightPresetApply(u8 idx) {
    f32 out[4];
    f32 rot[4];
    FldLightPreset *r;
    FldLightPresetDir *s;
    GphLight *l;
    s32 i;

    r = fldLightPresetTable;
    r += idx;
    camField[0].fogColor[0] = r->fogColor[0];
    camField[0].fogColor[1] = r->fogColor[1];
    camField[0].fogColor[2] = r->fogColor[2];
    camField[0].fogDist = r->fogDist;
    camField[0].fogMax = r->fogMax;
    camField[0].fogMin = r->fogMin;
    camField[0].clipNear = 1.0f;
    camField[0].clipFar = r->clipFar;
    l = GphLightGet(0);
    l->color[0] = r->ambient[0];
    l->color[1] = r->ambient[1];
    l->color[2] = r->ambient[2];
    l->color[3] = r->ambient[3];
    for (i = 0; i < 2; i++) {
        l = GphLightGet(i + 1);
        s = &r->dirs[i];
        rot[0] = s->angleX;
        rot[1] = s->angleY;
        DbgLightAngleSet(i, rot);
        VuDirFromAngles(out, rot);
        l->dir[0] = out[0];
        l->dir[1] = out[1];
        l->dir[2] = out[2];
        l->dir[3] = 0.0f;
        l->color[0] = s->color[0];
        l->color[1] = s->color[1];
        l->color[2] = s->color[2];
        l->color[3] = s->color[3];
        l->intensity = s->intensity;
    }
}
