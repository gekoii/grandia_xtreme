#include "common.h"
#include "protos/field.h"
#include "field/n26.h"


void McLoadReadDir(void) {
    McDirList *p;
    McReq *e;
    s32 n;

    if (mcLoadStateChanged != 0) {
        p = mcDirListPtr;
        p->max = 0x10;
        mcLoadDirReq = McReqGetDir((s32)mcLoadDirPattern, 0, 0x10, (s32)p->entries);
        return;
    }
    McGetCurPort();
    n = McReqGetStatus(mcLoadDirReq);
    e = McReqGet(mcLoadDirReq);
    if (McIsBusy() != 0) {
        return;
    }
    if (n == 0) {
        return;
    }
    if (n > 0 && e->result >= 0) {
        McLoadSetState(0x104);
        mcDirListPtr->count = e->result;
        McSlotsMarkUsed(mcDirListPtr);
    } else {
        McLoadSetState(0x308);
        McSlotTableClear();
    }
    /* s64 casts: these calls stay jal (no tail calls) */
    ((s64 (*)(s32))McReqFreeId)(mcLoadDirReq);
}
