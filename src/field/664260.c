#include "common.h"
#include "field/event.h"

void EvtScnrVarAdd(u8 type, u16 idx, s32 val) {
    u8 *base = evtScnrWork->vars;
    u8 *p = base + idx;

    if (base != NULL) {
        switch (type) {
        case 0:
            *p += val;
            break;
        case 1:
            *(u16 *)p += val;
            break;
        case 2:
            *(s32 *)p += val;
            break;
        }
    }
}
