#include "common.h"
#include "battle/misc.h"

void BtlFileInit(void) {
    btlFileLoadState = 0;
    btlFileLoadBuf = 0;
    btlModelReserveNum = 0;
}
