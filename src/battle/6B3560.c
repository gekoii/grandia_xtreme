#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void BtlChrModelCalcDraw(void *arg0) {
    if (btlChrModelCount < 0x100) {
        GphModelCalcAndDraw(arg0);
        *btlChrModelListPtr++ = arg0;
        btlChrModelCount++;
    }
}
