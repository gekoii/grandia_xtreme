#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void MemSet(void *, s32, s32);

void CamFollowInit(void) {
    CamTrailInit();
    MemSet(&camPadTurn, 0, 8);
    MemSet(&camFollowWork, 0, 0x610);
    MemSet(&camDebugTriBuf, 0, 0x1810);
    camPadTurnSpeed = 7.0f;
    camFollow->dist[CAM_FOLLOW_DIST_NORMAL] = 800.0f;
    camFollow->dist[CAM_FOLLOW_DIST_BACK] = 1100.0f;
    camFollow->dist[CAM_FOLLOW_DIST_FRONT] = 600.0f;
    camFollow->rate[0] = 0.6f;
    camFollow->rate[1] = 0.75f;
    camFollow->rate[2] = 1.25f;
    camFollow->rate[3] = 1.0f;
    camFollow->speed = 0.0f;
    camFollow->pitch = 0.13962631f;
    camFollow->unk4 = 0;
    camFollow->unk30 = 0.13962631f;
}
