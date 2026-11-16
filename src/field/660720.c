#include "common.h"
#include "field/event.h"

void EvtScnrSetScriptTable(s32 *arg0) {
    u32 i;
    s32 *hdr;

    for (i = 0; i < 0x100; i++) {
        evtScnrWork->scripts[i] = 0;
    }
    if (arg0 != NULL) {
        hdr = arg0;
        evtScnrWork->index = (u16 *)(hdr + 1);
        evtScnrWork->body = (u8 *)arg0 + *hdr;
        for (i = 0; i * 4 < *hdr - 4; i++) {
            evtScnrWork->scripts[i] = (u8 *)(evtScnrWork->index + i * 2);
        }
    } else {
        evtScnrWork->index = NULL;
        evtScnrWork->body = NULL;
    }
}
