#include "common.h"
#include "ptcl.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void PtclFldInit(void) {
    PtclFldWork **p = &ptclFldWork;
    void *t;

    t = FldMemAlloc(sizeof(PtclFldWork));
    *p = t;
    MemSet(t, 0, sizeof(PtclFldWork));
}
