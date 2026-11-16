#include "common.h"
#include "protos/field.h"
#include "field/event.h"

u8 *EvtScnrOpFlag(u8 *p) {
    s16 v;
    u16 idx;
    u32 op;
    u8 c;

    p = EvtScnrReadU16(p, &v);
    op = (u16)v >> 13;
    idx = v & 0x1FFF;
    switch (op) {
    case 0:
        EvtScnrFlagSet(idx, 0);
        break;
    case 1:
        EvtScnrFlagSet(idx, 1);
        break;
    case 2:
        c = evtScnrCur->condResult;
        if (c == 1) {
            if (!EvtScnrFlagGet(idx)) {
                evtScnrCur->condResult = c;
            } else {
                evtScnrCur->condResult = 0;
            }
        }
        break;
    case 3:
        c = evtScnrCur->condResult;
        if (c == 1) {
            if (EvtScnrFlagGet(idx)) {
                evtScnrCur->condResult = c;
            } else {
                evtScnrCur->condResult = 0;
            }
        }
        break;
    case 4:
        EvtScnrFlagGet(idx);
        break;
    }
    return p;
}
