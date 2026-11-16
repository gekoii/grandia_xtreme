#include "common.h"
#include "protos/field.h"
#include "camera.h"
#include "pad.h"


f32 CamPadTurnGet(Camera *a0, Pad *pad, s32 n, f32 a, f32 b) {
    return b + CamPadTurnInput(a0, pad, n, a);
}
