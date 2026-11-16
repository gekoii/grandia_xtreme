#include "common.h"
#include "field/event.h"

extern s32 evtScnrVarValue;

s32 EvtScnrVarGet(u8 type, u16 off) {
    u8 *base;
    u8 *p;

    base = evtScnrWork->vars;
    p = base + off;
    if (base == NULL) {
        return 0;
    }
    switch (type) {
    case 0:
        evtScnrVarValue = *p;
        break;
    case 1:
        evtScnrVarValue = *(u16 *)p;
        break;
    case 2:
        evtScnrVarValue = *(s32 *)p;
        break;
    }
    return evtScnrVarValue;
}
