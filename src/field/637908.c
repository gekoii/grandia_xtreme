#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MemSet(void *, s32, s32);

void FldMenuJunctionRecipeInit(void) {
    FldMenuJunctionRecipe *s = &fldMenuJunctionRecipe;

    MemSet(s, 0, 0xA4);
    FldMenuWinOpen(8, 0x140, 0x100, 0x1E0, 0x154, 0, 0, 0x400000);
    s->row = 1;
    s->col = 1;
}
