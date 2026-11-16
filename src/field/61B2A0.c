#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void EvtFaceWinClose(u8 i) {
    if (evtFaceWins[i].active != 0) {
        evtFaceWins[i].active = 0;
        FldMenuWinClose(i + 4, 0, 3);
    }
}
