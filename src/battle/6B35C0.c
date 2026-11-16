#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void BtlChrModelDraw(void *arg0) {
    if (btlChrModelCount < 0x100) {
        GphModelDraw(arg0);
        *btlChrModelListPtr++ = arg0;
        btlChrModelCount++;
    }
}
