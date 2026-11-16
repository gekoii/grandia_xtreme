#include "common.h"
#include "save.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapFloorAdvance(AmapWork *arg0) {
    if (sysSaveData.oldMapNo != sysSaveData.mapNo) {
        if (sysSaveData.oldMapNo != 0xA084) {
            amapFloorIndex = 0;
        } else {
            amapFloorIndex = 2;
        }
    }
    AmapCalcFloorNo(arg0->mapNo);
    AmapSetFloorIndex(amapFloorIndex);
    amapFloorIndex += 1;
    arg0->floor = amapFloorIndex;
}
