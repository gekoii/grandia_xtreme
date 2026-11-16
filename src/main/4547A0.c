#include "common.h"
#include "main/n08.h"

s32 McIsQueued(void) {
    return mcReqQueueHead != NULL;
}
