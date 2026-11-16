#include "common.h"
#include "main/n03.h"

void MemSet(void *, s32, s32);

void DbgTextClear(void) {
    MemSet(&dbgTextBuf, 0, sizeof(dbgTextBuf));
}
