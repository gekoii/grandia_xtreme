/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "field/state.h"
#include "field/n19.h"
#include "protos/field.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec;

void VuDirFromAngles(Vec *, Vec *);
void VuVecLerp(Vec *, Vec *, Vec *, f32);

void FldLightFadeUpdate(void) {
    Vec out;
    Vec a;
    Vec b;
    Vec o2;
    Vec rot;
    FldLightFade *sd;
    FldLightPreset *r0;
    FldLightPreset *r1;
    FldLightPresetDir *s0;
    FldLightPresetDir *s1;
    GphLight *l;
    s32 i;

    sd = &fldLightFade;
    if (sd->active == 0) {
        return;
    }
    sd->t += 1.0f / sd->frames;
    r0 = fldLightPresetTable;
    r0 += sd->from;
    r1 = fldLightPresetTable;
    r1 += sd->to;
    if (sd->t > 1.0f) {
        sd->t = 1.0f;
        sd->from = sd->to;
        sd->active = 0;
    }
    a.x = r0->fogColor[0];
    a.y = r0->fogColor[1];
    a.z = r0->fogColor[2];
    a.w = 0.0f;
    b.x = r1->fogColor[0];
    b.y = r1->fogColor[1];
    b.z = r1->fogColor[2];
    b.w = 0.0f;
    VuVecLerp(&out, &a, &b, sd->t);
    camField[0].fogColor[0] = out.x;
    camField[0].fogColor[1] = out.y;
    camField[0].fogColor[2] = out.z;
    a.x = r0->fogDist;
    a.y = r0->fogMax;
    a.z = r0->fogMin;
    a.w = 0.0f;
    b.x = r1->fogDist;
    b.y = r1->fogMax;
    b.z = r1->fogMin;
    b.w = 0.0f;
    VuVecLerp(&out, &a, &b, sd->t);
    camField[0].fogDist = out.x;
    camField[0].fogMax = out.y;
    camField[0].fogMin = out.z;
    camField[0].clipFar = r1->clipFar;
    a.x = r0->ambient[0];
    a.y = r0->ambient[1];
    a.z = r0->ambient[2];
    a.w = 0.0f;
    b.x = r1->ambient[0];
    b.y = r1->ambient[1];
    b.z = r1->ambient[2];
    b.w = 0.0f;
    VuVecLerp(&out, &a, &b, sd->t);
    l = GphLightGet(0);
    l->color[0] = out.x;
    l->color[1] = out.y;
    l->color[2] = out.z;
    l->color[3] = r1->ambient[3];
    for (i = 0; i < 2; i++) {
        l = GphLightGet(i + 1);
        s0 = &r0->dirs[i];
        s1 = &r1->dirs[i];
        a.x = s0->angleX;
        a.y = s0->angleY;
        a.z = s0->intensity;
        a.w = 0.0f;
        b.x = s1->angleX;
        b.y = s1->angleY;
        b.z = s1->intensity;
        b.w = 0.0f;
        VuVecLerp(&out, &a, &b, fldLightFade.t);
        rot.x = out.x;
        rot.y = out.y;
        l->intensity = out.z;
        DbgLightAngleSet(i, &rot);
        VuDirFromAngles(&o2, &rot);
        l->dir[0] = o2.x;
        l->dir[1] = o2.y;
        l->dir[2] = o2.z;
        l->dir[3] = 0.0f;
        a.x = s0->color[0];
        a.y = s0->color[1];
        a.z = s0->color[2];
        a.w = 0.0f;
        b.x = s1->color[0];
        b.y = s1->color[1];
        b.z = s1->color[2];
        b.w = 0.0f;
        VuVecLerp(&out, &a, &b, fldLightFade.t);
        l->color[0] = out.x;
        l->color[1] = out.y;
        l->color[2] = out.z;
        l->color[3] = s1->color[3];
    }
}
