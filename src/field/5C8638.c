#include "common.h"
#include "field/state.h"

void MemSet(void *, s32, s32);

void ChrObjClear(ChrObj *o) {
    void *a;
    void *b;

    b = o->motTable;
    a = o->trail;
    MemSet(o, 0, 0x730);
    o->motTable = b;
    o->trail = a;
}
