#include "common.h"
#include "field/event.h"

void EvtScnrSetVarBuf(s32 arg0) {
    evtScnrWork->vars = (u8 *)arg0;
}
