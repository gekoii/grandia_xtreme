#include "common.h"
#include "field/n15.h"

void MemSet(void *, s32, s32);

void CamScrInit(void) {
    MemSet(&camScrSaveStart, 0, 0x180);
    MemSet(&camScrSaveMap, 0, 0x180);
    camScrWait = 0;
    camScrFlags = 0;
    camScrTable = 0;
    camScrId = 0;
    camScrState = CAM_SCR_IDLE;
    camScrEndScript[0] = 0x1FFFF;
    MemSet(camScrMoveInterp, 0, 0x78);
    MemSet(camScrTurnInterp, 0, 0x78);
    MemSet(camScrPosInterp, 0, 0x78);
    MemSet(&camScrZoomInterp, 0, 0x28);
    MemSet(&camScrPersInterp, 0, 0x28);
    MemSet(&camScrLengthInterp, 0, 0x28);
    MemSet(&camScrMovePath, 0, 0x18);
    MemSet(&camScrPosPath, 0, 0x18);
}
