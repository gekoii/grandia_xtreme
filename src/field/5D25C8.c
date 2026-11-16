#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void MapJumpRequest(s32 arg0, s32 arg1) {
    if (mapLoadState.state != 0) {
        if (arg0 == 0xD000) {
            arg1 = 1;
            FldNewGameReset();
        }
        if (EvtSystemFlagGet(0x88) == 1) {
            if ((arg0 & 0xFF00) == 0x2000) {
                arg0 = (arg0 & 0xFF) | 0xE400;
            }
            if ((arg1 & 0xFF00) == 0xE400) {
                arg1 = (arg1 & 0xFF) + 0x2000;
            }
        }
        if (mapLoadState.state >= 2 || mapLoadState.state == MAP_LOAD_IDLE) {
            mapLoadState.state = MAP_LOAD_READ;
            mapNo.prev = mapNo.cur;
            mapNo.cur = arg0;
            if (MapHasVariant(arg0) != 0) {
                arg0 += mapVariantNo;
            }
            mapNo.file = arg0;
            mapNo.entrance = arg1;
        }
    }
}
