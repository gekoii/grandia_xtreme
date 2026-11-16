#include "common.h"
#include "battle/misc.h"

void GphModelUpdateGsRegs(s32);

void BtlChrModelListUpdateGsRegs(void) {
    s32 n;

    n = btlChrModelCount;
    btlChrModelListPtr = btlChrModelList;
    while (n > 0) {
        n--;
        GphModelUpdateGsRegs(*btlChrModelListPtr);
        btlChrModelListPtr++;
    }
}
