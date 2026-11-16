#include "common.h"
#include "field/event.h"

s32 EvtScnrFlagGet(u16 arg0) {
    return (evtScnrWork->vars[arg0 >> 3] >> (arg0 & 7)) & 1;
}
