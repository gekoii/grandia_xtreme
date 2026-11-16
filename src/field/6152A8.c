#include "common.h"
#include "field/n22.h"
#include "protos/field.h"

void AmapCalcFloorNo(s32 arg0) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (amapDungeonTable[i].mapId == arg0) {
            if (arg0 == 0xA008 && amapFloorIndex >= 2) {
                AmapSetFloorNo(amapDungeonTable[i].firstFloorNo + (u16)amapFloorIndex + 1);
            } else {
                AmapSetFloorNo(amapDungeonTable[i].firstFloorNo + (u16)amapFloorIndex);
            }
            return;
        }
    }
}
