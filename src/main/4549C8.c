#include "common.h"
#include "main/n08.h"

McReq *McReqLookup(s32 arg0) {
    if ((arg0 < 0) || (arg0 >= mcReqPool.count)) {
        return NULL;
    }
    return &mcReqPool.slots[arg0].req;
}
