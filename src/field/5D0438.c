#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamScrSetClip(Camera *arg0, s32 arg1, s32 arg2) {
    f32 v;

    v = CamScrArgFloat(arg2);
    if (arg2 == CAM_SCR_KEEP) {
        return;
    }
    if (arg2 == CAM_SCR_SAVESTART) {
        switch (arg1) {
        case 1:
            arg0->clipNear = camScrSaveStartClip14C;
            break;
        case 2:
            arg0->clipFar = camScrSaveStartClipF;
            break;
        }
    } else if (arg2 == CAM_SCR_SAVEMAP) {
        switch (arg1) {
        case 1:
            arg0->clipNear = camScrSaveMapClip14C;
            break;
        case 2:
            arg0->clipFar = camScrSaveMapClipF;
            break;
        }
    } else {
        switch (arg1) {
        case 0:
            arg0->fov = v;
            break;
        case 1:
            arg0->clipNear = v;
            break;
        case 2:
            arg0->clipFar = v;
            break;
        }
    }
}
