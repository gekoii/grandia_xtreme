#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void *memcpy(void *, void *, s32);

s32 EvtMsgGetLine(s32 line, u8 *out) {
    s32 n = 0;
    s32 count = 0;
    u8 *p;
    u8 *q;
    s32 len;

    p = EvtScnrFind(0x1100);
    if (p != NULL) {
        evtMsgCtl->flags &= ~8;
        p = EvtMsgSkipLeadOps(p);
        if (*p < 3) {
            *out = 0;
            return 0;
        }
        p += 2;
        len = 1;
        q = p;
        for (;;) {
            if (n == line && *q >= 0x20 && *q <= 0xEF) {
                count++;
            }
            if (*q == 0x1F) {
                if (n == line) {
                    break;
                }
                n++;
                len = 1;
                p = q + 1;
            }
            if (*q++ == 0x17 && *q++ == 0) {
                break;
            }
            len++;
        }
        {
            s32 k = len - 1;
            memcpy(out, p, k);
            out[k] = 0;
        }
    } else {
        *out = 0;
    }
    return count;
}
