#include "common.h"
#include "protos/main.h"
#include "main/n01.h"

void ChrPortraitAllocSlots(void) {
    s32 i;
    ChrPortrait *v;

    for (i = 0; i < 8; i++) {
        v = (ChrPortrait *)MemAlloc64(0x5B00);
        chrPortraitSlots.slots[i] = v;
        v->cur = -1;
    }
}
