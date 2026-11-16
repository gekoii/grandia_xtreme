#include "common.h"
#include "field/event.h"

void *memset(void *, s32, u32);

u8 *EvtMsgCopyRun(u8 *p, s32 mode) {
    u8 *q;
    s32 done;

    q = evtMsgCtl->name;
    done = 0;
    memset(q, 0, 0x100);
    if (evtMsgCtl->flags & 0x20) {
        /* identical arms: the original still tests the condition (the branch is in the code) */
        if ((evtMsgCtl->cursorY >> 1) == evtMsgCtl->choice) {
            evtScnrWork->textColor = 0xF0F;
        } else {
            evtScnrWork->textColor = 0xF0F;
        }
    }
    do {
        if (p[1] >= 0xF0) {
            *q++ = *p++;
        }
        *q++ = *p++;
        switch (*p) {
        case 0x1B:
            done |= 0x8008;
            break;
        case 0:
        case 0x17:
        case 0x1A:
            done |= 8;
            break;
        case 0x1F:
            break;
        default:
            if (*p < 0x20) {
                /* identical arms: the original still tests the condition (the branch is in the code) */
                if (mode) {
                    evtMsgCtl->flags |= 2;
                } else {
                    evtMsgCtl->flags |= 2;
                }
            }
            break;
        }
    } while (!(done & 0xA));
    return p;
}
