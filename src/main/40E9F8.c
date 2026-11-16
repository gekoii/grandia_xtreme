#include "common.h"
#include "sys.h"
#include "protos/main.h"
#include "main/n01.h"

void ChrPortraitLoad(s32 arg0) {
    s32 id;
    u32 i;

    id = FileGetSizeAligned(arg0);
    MemHeapSelect(sysFaceHeap);
    chrPortraitSlots.faceOffsets = MemAlloc32(id);
    FileRead(arg0, chrPortraitSlots.faceOffsets);
    chrPortraitSlots.faceCount = *chrPortraitSlots.faceOffsets >> 2;
    for (i = 0; i < chrPortraitSlots.faceCount; i++) {
        chrPortraitSlots.faceOffsets[i] += (u32)chrPortraitSlots.faceOffsets;
    }
}
