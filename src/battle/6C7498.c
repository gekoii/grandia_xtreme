#include "common.h"
#include "sys.h"
#include "protos/battle.h"

s32 BtlAlloc32(s32 arg0) {
    s32 cur;
    s32 r;

    cur = MemHeapGetCurrentId();
    if (cur == sysBattleHeap) {
        r = MemAlloc32(arg0);
    } else {
        MemHeapSelect(sysBattleHeap);
        r = MemAlloc16C(arg0);
        MemHeapSelect(cur);
    }
    return r;
}
