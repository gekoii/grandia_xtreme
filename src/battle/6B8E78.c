#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "main/n05.h"
#include "battle/n32.h"


s32 BtlModelPoolAlloc(ChrModelPool *st) {
    s32 size;
    s32 i;
    s32 buf;

    size = 0;
    for (i = 0; i < btlModelReserveNum; i++) {
        size += ChrModelCalcInstanceSize(btlModelReserveData[i]) * btlModelReserveCount[i];
    }
    btlModelReserveNum = 0;
    buf = MemAlloc128(size);
    if (buf == 0) {
        return 0;
    }
    ChrModelPoolInit(st, buf, size);
    ChrModelPoolSet(st);
    return buf;
}
