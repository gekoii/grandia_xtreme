#include "common.h"
#include "field/n15.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void ChrEnemyWorkPoolInit(void) {
    ChrEnemyWorkPool **q = &chrEnemyWorkPool;
    void *p;

    p = FldMemAlloc(0x1004);
    *q = p;
    MemSet(p, 0, 0x1004);
}
