#include "common.h"
#include "protos/field.h"
#include "field/event.h"

u8 *EvtScnrOpVar(u8 *p, s8 type) {
    s32 val;
    s16 b;
    s16 a;
    s16 v;
    u8 mode = *p;
    u16 idx;
    u32 op;

    p = EvtScnrReadU16(p + 1, &v);
    op = (u16)v >> 13;
    idx = v & 0x1FFF;
    if (mode == 0) {
        switch (type) {
        case 0:
            val = *p++;
            break;
        case 1:
            p = EvtScnrReadU16(p, &a);
            val = (u16)a;
            break;
        case 2:
            p = EvtScnrReadU32(p, &val);
            break;
        }
    } else {
        p = EvtScnrReadU16(p, &b);
        switch (type) {
        case 0:
            val = EvtScnrVarGet(0, b);
            break;
        case 1:
            val = EvtScnrVarGet(1, b);
            break;
        case 2:
            val = EvtScnrVarGet(2, b);
            break;
        }
    }
    switch (op) {
    case 0:
        EvtScnrVarSet(type, idx, val);
        break;
    case 1:
        EvtScnrVarAdd(type, idx, val);
        break;
    default:
        if (evtScnrCur->condResult == 1) {
            evtScnrCur->condResult = EvtScnrVarCompare(type, idx, val, op - 2);
        }
        break;
    }
    return p;
}
