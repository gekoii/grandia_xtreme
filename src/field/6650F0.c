#include "common.h"
#include "field/event.h"

void EvtMsgWinCloseSlot(s32 arg0) {
    evtMsgWins[arg0 & 0xFF].flags = 0xA;
}
