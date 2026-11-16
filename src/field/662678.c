/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtMsgPromptShow(u16 arg0, u16 arg1) {
    if (!(evtMsgCtl->flags & 0x400) && evtMsgCtl->wait == 0.0f) {
        if (arg1 == 0) {
            /* cast: u8 first argument (the definition takes s8) and an s32 return, so the calls stay jal */
            ((s32 (*)(u8, s16, s16))EvtMsgPromptSet)(arg0, 0x230, 0x88);
        } else if (arg1 == 1) {
            ((s32 (*)(u8, s16, s16))EvtMsgPromptSet)(arg0, 0x230, 0x1A8);
        }
    }
}
