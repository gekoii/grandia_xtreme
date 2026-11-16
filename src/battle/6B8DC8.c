#include "common.h"
#include "battle/misc.h"

void BtlFileLoadSetPending(s32 arg0) {
    btlFileLoadState = 1;
    if ((arg0 << 0x18) != 0) {
        btlFileLoadBuf = 1;
    }
}

void BtlFileSetLoadedCallback(s32 arg0) {
    btlFileLoadedCallback = (void (*)(void))arg0;
}
