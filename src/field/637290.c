#include "common.h"
#include "main/n03.h"
#include "protos/field.h"

void MemSet(void *, s32, s32);
extern u8 fldMenuMcCheckWork[];

void FldMenuMcCheckInit(void) {
    McPortState *s;

    s = McGetPortState(0);
    s->syncResult = -1;
    s->state = 0;
    s = McGetPortState(1);
    s->syncResult = -1;
    s->state = 0;
    McSpaceCheckReset();
    MemSet(fldMenuMcCheckWork, 0, 0xC);
}
