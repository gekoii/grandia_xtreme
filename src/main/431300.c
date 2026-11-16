#include "common.h"
#include "protos/main.h"
#include "camera.h"

void CamCalcMatrix();

s32 CamApply(Camera *arg0) {
    CamCalcMatrix();
    GphViewSetClip(arg0->viewNo, arg0->clipNear, arg0->clipFar);
    GphViewSetFovScaled(arg0->viewNo, arg0->fov, arg0->zoom);
    /* cast: EABI argument-load order of the (s32, f32 x3, s32 x3) declaration */
    ((void (*)(s32, f32, f32, f32, s32, s32, s32))GphViewSetFog)(arg0->viewNo, arg0->fogDist, arg0->fogMax, arg0->fogMin, arg0->fogColor[0], arg0->fogColor[1], arg0->fogColor[2]);
    GphViewSelect(arg0->viewNo, arg0->roll);
    CamSetCurrent(arg0);
    return 0;
}
