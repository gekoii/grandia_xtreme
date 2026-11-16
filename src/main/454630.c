#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

void McDriverInit(void) {
    McPortState *p;
    s32 i;

    sceMcInit();
    mcPollInterval = 0x3C;
    McReset();
    i = 0;
    p = mcPortState;
    do {
        p->port = i;
        p->state = MC_STATE_NONE;
        i++;
        p->slot = 0;
        p->formatted = 0;
        p->type = 0;
        p->lastCmd = 0;
        p->result = 0;
        p->busyFrames = 0;
        p->freeKB = 0;
        p->unk24 = 0;
        p++;
    } while (i < 2);
    mcCurPort = mcPortState;
}
