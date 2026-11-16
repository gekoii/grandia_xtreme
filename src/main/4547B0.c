#include "common.h"
#include "main/n08.h"

void *McQueuePop(void) {
    McReq *ret = NULL;
    s32 i;

    if (mcReqQueue.slots[0] != NULL) {
        ret = mcReqQueue.slots[0];
        for (i = 0; i < mcReqQueue.count; i++) {
            mcReqQueue.slots[i] = mcReqQueue.slots[i + 1];
        }
        mcReqQueue.slots[31] = NULL;
    }
    return ret;
}
