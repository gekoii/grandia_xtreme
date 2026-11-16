#include "common.h"
#include "field/event.h"

void EvtMsgDispUpdate(void) {
    u16 temp_v1;

    if (evtMsgCtl->state != 0xFF) {
        temp_v1 = evtMsgCtl->flags;
        if (temp_v1 & 0x80) {
            evtMsgCtl->flags = (u16) (temp_v1 & 0xFF7F);
        }
    }
}
