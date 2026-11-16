#include "common.h"
#include "main/n08.h"

s32 McReqFree(McReq *arg0) {
    McReqSlot **pe;
    McReqPool *t = &mcReqPool;
    McReqSlot *e;
    e = t->slots;
    pe = &e;
    e += arg0->id;
    if ((*pe)->used == 0) {
        return -1;
    }
    e->used = 0;
    return 0;
}
