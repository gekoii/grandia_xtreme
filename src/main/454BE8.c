#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

void McUpdate(void) {
    s32 stat[2];
    s32 thresh;
    s32 r;
    McPortState *q;
    McReq *pp;
    McReq **ppp;

    stat[0] = 0;
    thresh = mcPollInterval;
    stat[1] = 0;
    r = sceMcSync(1, &stat[0], &stat[1]);
    if (r == 0) {
        mcBusy = 1;
    } else {
        mcBusy = 0;
    }
    mcSyncResult = r;
    q = McGetPortState(mcSyncPort);
    q->syncResult = mcSyncResult;
    mcPollFrames++;
    if (stat[1] < -10) {
        q->state = 0;
    }
    if (r == 0) {
        mcPollFrames = 0;
        mcBusyFrames++;
        return;
    }
    /* while (...) { ...; break; } and the goto dispatch/dflt pair: the original's
     * switch layout (dispatch after the default body, see matching-tips) */
    while (r > 0) {
        q->lastCmd = stat[0];
        q->result = stat[1];
        q->busyFrames = mcBusyFrames;
        mcBusyFrames = 0;
        ppp = &mcCurReq;
        do {} while (0); /* scheduling barrier: stops a li 1 being hoisted above it */
        if ((*ppp) != NULL && (*ppp)->port == q->port && (*ppp)->slot == q->slot) {
            (*ppp)->status = 2;
            pp = (*ppp);
            (*ppp) = NULL;
            pp->result = stat[1];
            if (stat[1] < 0) {
                McQueueCancelAll();
            }
        }
        if (stat[0] == 1) {
            if (stat[1] < -10) {
                q->state = 0;
            } else {
                goto dispatch;
            dflt:
                if (stat[1] >= 0 && q->state == 0) {
                    q->state = 1;
                } else if (q->state == 1) {
                    q->state = 2;
                }
                break;
            dispatch:
                if (stat[1] == -1) {
                    q->state = 1;
                    q->formatted = 1;
                } else if (stat[1] != -2) {
                    goto dflt;
                } else {
                    q->state = 1;
                    q->formatted = 0;
                }
            }
        }
        break;
    }
    q = McGetCurPort();
    mcSyncPort = q->port;
    switch (q->state) {
    case 0:
        if (mcPollFrames >= thresh) {
            sceMcGetInfo(q->port, q->slot, NULL, NULL, NULL);
            mcBusy = 0;
            q->state = 0;
            mcPollFrames = 0;
        }
        break;
    case 1:
        sceMcGetInfo(q->port, q->slot, &q->type, &q->freeKB, &q->formatted);
        mcBusy = 1;
        mcPollFrames = 0;
        mcSyncResult = 0;
        break;
    case 2:
        if (McIsQueued() != 0) {
            McExecNext();
            break;
        }
        /* do-while(0): branch layout / alignment of the poll test below */
        do {
        if (r >= 0 && q->lastCmd != 1) {
            sceMcGetInfo(q->port, q->slot, NULL, &q->freeKB, NULL);
            mcBusy = 1;
            mcPollFrames = 0;
            mcSyncResult = 0;
        }
        } while (0);
        if (mcPollFrames >= thresh) {
            sceMcGetInfo(q->port, q->slot, NULL, NULL, NULL);
            mcSyncResult = 0;
            mcBusy = 1;
            mcPollFrames = 0;
        }
        break;
    }
}
