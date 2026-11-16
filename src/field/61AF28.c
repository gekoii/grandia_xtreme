#include "common.h"
#include "field/n22.h"

extern void *FldMemAlloc(s32 size);
extern void MemSet(void *, s32, s32);

void EvtFaceWinInit(void) {
    EvtFaceWin **p = &evtFaceWins;
    void *t;

    t = FldMemAlloc(0x85E0);
    *p = t;
    MemSet(t, 0, 0x85E0);
}
