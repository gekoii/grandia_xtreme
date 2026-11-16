#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MemSet(void *, s32, s32);

void FldMenuSkillmanInit(void) {
    FldMenuSkillman *w = &fldMenuSkillman;

    MemSet(w, 0, 0x38);
    FldMenuWinOpen(8, 0x112, 0x5A, 0x1E0, 0x78, 0, 0, 0x400000);
    FldMenuWinOpen(9, 0x112, 0x5A, 0x1E0, 0x78, 0, 1, 0x400000);
    w->t = 0;
    w->last = -1;
    w->ripple = FldMenuGetRippleWork();
}
