#include "common.h"
#include "pad.h"
#include "field/event.h"
#include "protos/field.h"

extern u16 *evtScnrInput;

void EvtScnrInputUpdate(Pad *pad) {
    u16 *p;
    u16 v;

    p = EvtScnrGetInput();
    if (p != NULL) {
        p[0] = 0;
        p[1] = 0;
        if (pad->trigger & 0x20) {
            *evtScnrInput |= 0x2000;
        }
        if (pad->trigger & 0x40) {
            *evtScnrInput |= 0x4000;
        }
        if (pad->trigger & 0x1000) {
            *evtScnrInput |= 1;
        }
        if (pad->trigger & 0x4000) {
            *evtScnrInput |= 2;
        }
        /* FldSndSePlay through an s64 cast (3 calls): keeps them out of tail position (jal) */
        if (EvtScnrIsRunning() != 0 && ((v = evtMsgCtl->flags) & 8)) {
            if (v & 0x20) {
                switch (*evtScnrInput) {
                case 1:
                case 2:
                    ((s64 (*)(s32, s32, s32))FldSndSePlay)(0, 0x7D1, 0);
                    break;
                case 0x2000:
                    ((s64 (*)(s32, s32, s32))FldSndSePlay)(0, 0x7D2, 0);
                    break;
                }
            } else if (*evtScnrInput & 0x6000) {
                ((s64 (*)(s32, s32, s32))FldSndSePlay)(0, 0x7EE, 0);
            }
        }
    }
}
