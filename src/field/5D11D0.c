#include "common.h"
#include "field/n15.h"

void TxtMsgCursorShow(u8 arg0, s16 arg1, s16 arg2, u8 arg3) {
    TxtMsgCursor *e;
    if (txtMsgCursorWork[arg0].used == 0) {
        txtMsgCursorWork[arg0].u = 0;
        txtMsgCursorWork[arg0].v = 0;
        txtMsgCursorWork[arg0].used = 1;
    }
    txtMsgCursorWork[arg0].kind = arg3;
    e = (TxtMsgCursor *)(arg0 * 0x90 + (u32)txtMsgCursorWork);
    e->x = arg1;
    e->y = arg2;
}
