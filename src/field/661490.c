#include "common.h"
#include "field/event.h"

s32 EvtMsgReadChar(u16 *out) {
    s32 n;
    u8 *p;
    u8 c;

    n = 1;
    p = evtScnrWork->pc;
    c = *p++;
    *out = c;
    if (c >= 0x20 && *p >= 0xF0) {
        n = 2;
        *out = c | (*p++ << 8);
    }
    evtScnrWork->pc = p;
    return n;
}
