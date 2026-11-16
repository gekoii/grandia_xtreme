#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"

void VuVecCopy(VuVec4 *, VuVec4 *);
void VuVecLerp(VuVec4 *, VuVec4 *, VuVec4 *, f32);

void CamSetTransitPoint(VuVec4 *to, f32 a) {
    VuVec4 tmp;

    camCtl.unkC = 0.0f;
    CamPitchYawToDir(&tmp, 0.13962634f, a);
    VuVecCopy((VuVec4 *)camCtl.transitFrom, (VuVec4 *)camField[0].eye);
    VuVecLerp((VuVec4 *)camCtl.transitMid, (VuVec4 *)camCtl.transitFrom, to, 0.5f);
}
