#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

typedef struct {
    f32 x;
    f32 y;
} Rot;

extern void VuDirFromAngles(VuVec4 *, Rot *);
extern void GphShadowCalcMtx(FMATRIX *, VuVec4 *);

void FldShadowSetup(s32 arg0) {
    Rot rot;
    VuVec4 dir;
    VuVec4 q;

    rot.x = 90.0f;
    rot.y = 45.0f;
    VuDirFromAngles(&dir, &rot);
    GphShadowCalcMtx(&fldShadowMtx, &dir);
    q.x = -255.0f;
    q.y = -255.0f;
    q.z = 32.0f;
    q.w = 60.0f;
    GphShadowSetColor(&q);
}
