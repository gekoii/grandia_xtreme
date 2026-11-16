#include "common.h"
#include "field/n22.h"

extern void MemSet(void *, s32, s32);

void AmapWorkClear(void) {
    MemSet(&amapWork, 0, sizeof(AmapWork));
}
