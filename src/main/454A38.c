#include "common.h"
#include "main/n08.h"

s32 McQueuePush(void *arg0) {
    s32 i;
    for (i = 0; i < mcReqQueue.count; i++) {
        if (mcReqQueue.slots[i] == NULL) {
            mcReqQueue.slots[i] = arg0;
            return 1;
        }
    }
    return 0;
}
