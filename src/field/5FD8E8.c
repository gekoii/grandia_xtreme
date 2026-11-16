#include "common.h"
#include "field/n20.h"

EvtCharSelWork *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void EvtCharSelInit(void) {
    EvtCharSelWork **q = &evtCharSelWork;
    EvtCharSelWork *p;

    p = FldMemAlloc(0x180);
    *q = p;
    MemSet(p, 0, 0x180);
}
