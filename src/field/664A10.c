#include "common.h"
#include "field/event.h"

void EvtMsgCtlSetFlags(s32 arg0, s32 arg1) {
    u16 temp_a2;

    temp_a2 = arg0 & 0xFFFF;
    switch (arg1) {                                 /* irregular */
    case 0:
        evtMsgCtl->flags = temp_a2;
        return;
    case 1:
        evtMsgCtl->flags = (u16) (temp_a2 | evtMsgCtl->flags);
        return;
    case 2:
        evtMsgCtl->flags = (u16) (temp_a2 & evtMsgCtl->flags);
        return;
    }
}
