#include "common.h"
#include "field/n27.h"

void EvtMsgTileGridClear(void) {
    u16 *p = evtMsgTileGrid;
    s32 i;

    for (i = 0x4AF; i >= 0; i--) {
        *p++ = 0;
    }
}
