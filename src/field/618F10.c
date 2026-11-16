#include "common.h"
#include "sys.h"
#include "protos/field.h"
#include "field/n22.h"

void FldMemInit(s32 size) {
    s32 v;
    u32 *p1;
    u32 *p2;

    p1 = &fldMemTop;
    p2 = &fldMemSubTop;
    MemHeapSelect(sysState.dataHeap);
    *p1 = v = MemAlloc128(size);
    fldMemEnd = v + size;
    MemHeapSelect(sysState.battleMagicHeap);
    size = 0x11FF00;
    MemHeapReset(sysState.battleMagicHeap);
    *p2 = v = MemAlloc128(0x11FF00);
    fldMemSubEnd = v + size;
    MemHeapSelect(sysState.dataHeap);
    FldMemReset();
}
