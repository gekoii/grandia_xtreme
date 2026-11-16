#include "common.h"
#include "main/n05.h"

void MemSet(void *, s32, s32);

void GphEnvMapClear(void) {
    MemSet(gphEnvMapTable, 0, 0xC00);
}
