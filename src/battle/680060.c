#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "sys.h"


void BtlInit(void) {
    btlFromField = SysIsBattleBoot() != 1;
    MemHeapSelect(sysState.battleMagicHeap);
    MemFree(MemAlloc16A(0x20));
    MemHeapSelect(sysState.battleHeap);
    BtlDebugCfgLoad();
}
