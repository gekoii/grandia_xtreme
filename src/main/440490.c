#include "common.h"
#include "protos/main.h"
#include "main/n06.h"

extern s32 memCurHeapId;

void MemHeapSelect(s32 arg0) {
    MemHeap *temp_v0;

    temp_v0 = MemHeapFind(arg0);
    if (temp_v0 != 0) {
        memCurHeap = temp_v0;
        memCurHeapId = arg0;
    }
}

s32 MemHeapGetCurrentId(void) {
    return memCurHeapId;
}
