#include "common.h"
#include "field/n15.h"

s32 CamScrIsRunning(void) {
    return camScrState != CAM_SCR_IDLE;
}
