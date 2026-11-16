#include "common.h"
#include "battle/camera.h"

void BtlCamSetCalcMode(s32 arg0) {
    switch (arg0) {
    case 0:
        btlCamera->mode = 0x39;
        break;
    case 1:
        btlCamera->mode = 0x3A;
        break;
    case 2:
        btlCamera->mode = 0x23;
        break;
    }
}
