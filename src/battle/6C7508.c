#include "common.h"
#include "sys.h"
#include "protos/battle.h"

s32 BtlAlloc128(s32 arg0) {
    s32 cur;
    s32 r;

    cur = MemHeapGetCurrentId();
    if (cur == sysBattleHeap) {
        r = MemAlloc128(arg0);
    } else {
        MemHeapSelect(sysBattleHeap);
        r = MemAlloc128(arg0);
        MemHeapSelect(cur);
    }
    return r;
}
