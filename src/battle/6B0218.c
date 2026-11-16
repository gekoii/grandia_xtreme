#include "common.h"
#include "battle/camera.h"
#include "camera.h"

void BtlCamQuakeStop(s32 arg0) {

    if (arg0 <= 0) {

        btlCamera->shakeAmp[0] = 0;
        btlCamera->shakeFlags &= 0xFFFE;
        btlCamera->shakeAmp[1] = 0;
        btlCamera->shakeAmp[2] = 0;
        btlCamera->shakeAmp[3] = 0;
        btlCamera->shakeRand[0] = 0;
        btlCamera->shakeRand[1] = 0;
        btlCamera->shakeRand[2] = 0;
        btlCamera->shakeRand[3] = 0;
        btlCamera->shakeTick = 0;
    } else {

        btlCamera->shakeAmp2[0] = 0;
        btlCamera->shakeMode |= 0x18;
        btlCamera->shakeAmp2[1] = 0;
        btlCamera->shakeAmp2[2] = 0;
        btlCamera->shakeAmp2[3] = 0;
        btlCamera->shakeRand2[0] = 0;
        btlCamera->shakeRand2[1] = 0;
        btlCamera->shakeRand2[2] = 0;
        btlCamera->shakeRand2[3] = 0;
        btlCamera->shakeTime = arg0;
        btlCamera->shakePeriod = arg0;
    }
}
