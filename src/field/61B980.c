#include "common.h"
#include "field/state.h"

extern void *FldMemAlloc(s32 size);
extern void MemSet(void *, s32, s32);

void FldSpotInit(void) {
    FldSpotWork **p = &fldSpotWork;
    void *t;

    t = FldMemAlloc(sizeof(FldSpotWork));
    *p = t;
    MemSet(t, 0, sizeof(FldSpotWork));
}
