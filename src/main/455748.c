#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

void McScanPorts(void) {
    s32 st[2];
    s32 r;
    McPortState *p;
    McPortState *e;
    McPortState *end;
    s32 one;

    st[0] = 0;
    st[1] = 0;
    r = sceMcSync(1, &st[0], &st[1]);
    if (r == 0) {
        mcBusy = 1;
    } else {
        mcBusy = 0;
    }
    mcSyncResult = r;
    p = McGetPortState(mcSyncPort);
    if (st[1] < -10) {
        p->state = 0;
    }
    if (r == 0) {
        mcScanBusyFrames++;
        return;
    }
    /* while (...) { ...; break; } and the goto dispatch/dflt pair: the original's
     * switch layout (dispatch after the default body, see matching-tips) */
    while (r > 0) {
        p->lastCmd = st[0];
        p->result = st[1];
        p->busyFrames = mcScanBusyFrames;
        mcScanBusyFrames = 0;
        do {} while (0); /* scheduling barrier: stops a li 1 being hoisted above it */
        if (st[0] == 1) {
            if (st[1] < -10) {
                p->state = 0;
            } else {
                goto dispatch;
            dflt:
                if (st[1] >= 0 && p->state == 0) {
                    p->state = 1;
                } else if (p->state == 1) {
                    p->state = 2;
                }
                break;
            dispatch:
                if (st[1] == -1) {
                    p->state = 1;
                    p->formatted = 1;
                } else if (st[1] != -2) {
                    goto dflt;
                } else {
                    p->state = 1;
                    p->formatted = 0;
                }
            }
        }
        break;
    }
    one = 1;
    e = mcPortState;
    end = e + 2;
    for (; (s32)e < (s32)end; e++) {
        if (McPortPoll(e) == one) {
            break;
        }
        e->pollTimer++;
    }
}
