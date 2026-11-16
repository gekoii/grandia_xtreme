#include "common.h"
#include "field/event.h"

void *memset(void *, s32, u32);

u8 *EvtMsgCopyChar(u8 *p) {
    EvtMsgCtl *c;

    c = evtMsgCtl;
    memset(c->name, 0, 0x100);
    if (p[1] >= 0xF0) {
        c->name[0] = *p++;
        c->name[1] = *p++;
    } else {
        c->name[0] = *p++;
    }
    return p;
}
