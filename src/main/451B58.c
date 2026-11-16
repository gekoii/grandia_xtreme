#include "common.h"
#include "main/n07.h"

void MemSet(void *, s32, s32);

void GphWipeInit(void) {
    MemSet(&gphWipe, 0, 6);
    MemSet(&gphWipeSprites, 0, 0x200);
}
