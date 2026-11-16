#include "common.h"
#include "field/event.h"

void EvtScnrFlagSet(u16 arg0, s32 arg1) {
    evtScnrWork->vars[arg0 >> 3] &= ~(1 << (arg0 & 7));
    evtScnrWork->vars[arg0 >> 3] |= (arg1 & 1) << (arg0 & 7);
}
