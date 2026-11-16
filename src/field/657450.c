/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n25.h"

extern void VuVecCopy(void *, void *);
extern void VuBezier3(FVECTOR *, FVECTOR *, f32);

void FldTitleCamFlyUpdate(void) {
    FVECTOR a[4];
    FVECTOR b[4];
    FVECTOR out;
    s32 set;
    s32 idx;
    s32 k;

    set = 0;
    switch (EvtGetMapNo()) {
    case 0x4050:
        set = 1;
        break;
    case 0x4820:
        set = 2;
        break;
    case 0x4810:
        set = 4;
        break;
    case 0x5020:
        set = 5;
        break;
    case 0x5000:
        break;
    case 0x5830:
        set = 3;
        break;
    }
    fldTitleWork->t += 0.001f;
    if (fldTitleWork->t >= 1.0f) {
        fldTitleWork->t = 0.0f;
        fldTitleWork->shot++;
    }
    idx = fldTitleWork->order[fldTitleWork->shot] * 8 + set * 64;
    for (k = 0; k < 4; k++) {
        VuVecCopy(b[k], fldTitleWork->shotKeys[idx + k * 2]);
        VuVecCopy(a[k], fldTitleWork->shotKeys[idx + 1 + k * 2]);
    }
    VuBezier3(&out, a, fldTitleWork->t);
    VuVecCopy(camField[0].lookAt, out);
    VuBezier3(&out, b, fldTitleWork->t);
    camField[0].pitch = out[0];
    camField[0].yaw = out[1];
    camField[0].roll = out[2];
    camField[0].fov = 55.0f;
    camField[0].dist = 700.0f;
    camField[0].zoom = 1.0f;
}
