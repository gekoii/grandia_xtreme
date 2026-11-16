#include "common.h"
#include "field/event.h"

void *EvtMsgCodeSetIdBase(u8 *arg0) {
    evtScnrWork->idBase = *arg0 << 8;
    return arg0 + 1;
}
