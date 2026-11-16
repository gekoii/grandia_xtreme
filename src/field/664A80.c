#include "common.h"
#include "field/event.h"

EvtMsgWin *EvtMsgWinGet(s32 arg0) {
    return &evtMsgWins[arg0];
}
