#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

s32 McPortPoll(McPortState *arg0) {
    s32 t;

    t = mcPollInterval;
    mcSyncPort = 0;
    switch (arg0->state) {
    case 0:
        if (t < arg0->pollTimer) {
            sceMcGetInfo(arg0->port, 0, NULL, NULL, NULL);
            mcBusy = 0;
            arg0->state = 0;
            arg0->pollTimer = 0;
            mcSyncPort = arg0->port;
            mcSyncResult = 0;
            return 1;
        }
        break;
    case 1:
        sceMcGetInfo(arg0->port, 0, &arg0->type, &arg0->freeKB, &arg0->formatted);
        mcBusy = 1;
        arg0->pollTimer = 0;
        mcSyncPort = arg0->port;
        mcSyncResult = 0;
        return 1;
    case 2:
        if (McIsQueued() == 0 && t < arg0->pollTimer) {
            if (arg0->lastCmd == 1) {
                sceMcGetInfo(arg0->port, 0, NULL, NULL, NULL);
            } else {
                sceMcGetInfo(arg0->port, arg0->slot, NULL, &arg0->freeKB, NULL);
            }
            mcBusy = 1;
            arg0->pollTimer = 0;
            mcSyncPort = arg0->port;
            mcSyncResult = 0;
            return 1;
        }
        break;
    }
    return 0;
}
