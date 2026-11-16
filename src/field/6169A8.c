#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void AmapFloorBannerStart(s32 arg0) {
    s32 t;

    t = EvtGetMapNo() & 0xFF00;
    if (t == 0x6000 || t == 0xA000 || t == 0xD800) {
        amapState->bannerOn = 1;
        amapState->bannerFloor = amapSaveFloorNo;
        amapState->bannerAlpha = 0x80;
        amapState->bannerT = 0.0f;
    }
}
