#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamScrReset(void *arg0) {
    s32 i;

    CamScrSaveCamera(0);
    for (i = 0; i < 3; i++) {
        camScrMoveInterp[i].active = 0;
        camScrTurnInterp[i].active = 0;
        camScrPosInterp[i].active = 0;
    }
    camScrZoomInterp.active = 0;
    camScrPersInterp.active = 0;
    camScrLengthInterp.active = 0;
    camScrMovePath.active = 0;
    camScrPosPath.active = 0;
}
