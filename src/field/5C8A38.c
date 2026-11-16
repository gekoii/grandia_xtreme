#include "common.h"
#include "field/n14.h"

void MemSet(void *, s32, s32);

void TxtMsgWinStateClear(void) {
    MemSet(txtMsgWinOpened, 0, 4);
}
