#include "common.h"
#include "battle/misc.h"

void BtlModelPoolReserve(s32 arg0, u8 arg1) {
    if (btlModelReserveNum < 0x20) {
        btlModelReserveData[btlModelReserveNum] = arg0;
        btlModelReserveCount[btlModelReserveNum] = arg1;
        btlModelReserveNum++;
    }
}
