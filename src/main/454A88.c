#include "common.h"
#include "main/n08.h"

void McQueueCancelAll(void) {
    s32 i;
    McReq *p;

    for (i = 0; i < mcReqQueue.count; i++) {
        p = mcReqQueue.slots[i];
        if (p != NULL) {
            mcReqQueue.slots[i] = NULL;
            p->status = -1;
        }
    }
}
