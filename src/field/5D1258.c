/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n15.h"

void TxtMsgCursorUpdate(void) {
    s32 i;

    if (evtMsgCursor->on != 0 && evtMsgWins->layout != 0xFF) {
        TxtMsgCursorShow(0, evtMsgCursor->x, evtMsgCursor->y, 0);
        i = 0;
        /* the loop is placed after the hide path (structured forms are 49-55 words off) */
        goto loop;
    }
    TxtMsgCursorHide(0);
    return;
loop:
    do {
        if (txtMsgCursorWork[i].used != 0) {
            txtMsgCursorWork[i].u += 0.02f;
            if (txtMsgCursorWork[i].u >= 1.0f) {
                txtMsgCursorWork[i].u -= 1.0f;
            }
            txtMsgCursorWork[i].v += 0.005f;
            if (txtMsgCursorWork[i].v >= 1.0f) {
                txtMsgCursorWork[i].v -= 1.0f;
            }
        }
        i++;
    } while (i < 2);
}
