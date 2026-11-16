#include "common.h"
#include "field/n20.h"

void MemSet(void *, s32, s32);

void ChrListClear(void) {
    MemSet(&chrList, 0, 0x28);
}
