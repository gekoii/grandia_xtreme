#include "common.h"
#include "sys.h"
#include "protos/battle.h"

void BtlPrepare(void) {
    MemHeapSelect(sysBattleHeap);
    AiSpellAllowInit();
}
