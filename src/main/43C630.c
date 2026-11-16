#include "common.h"

extern s32 gphPrimSecondFlag;
extern s32 gphPrimFrameCount;

void GphPrimSecondTick(void) {
    f32 a[4];

    gphPrimSecondFlag = 0;
    gphPrimFrameCount++;
    if (gphPrimFrameCount % 60 == 0) {
        gphPrimSecondFlag = 1;
    }
    a[0] = a[1] = a[2] = 0.0f; a[3] = 400.0f; a[4] = a[5] = 0.0f; a[6] = 1000.0f; a[7] = 400.0f;
}
