#include "common.h"
#include "protos/main.h"
#include "ptcl.h"


void PtclStarterSpawn(s32 arg0, s32 arg1, PtclStarterEntry *arg2) {
    PtclGenRequest req;
    PtclCtrl *rec;
    PtclStarterSlot *slot;
    s32 h;
    s32 size;
    s8 c;

    rec = (PtclCtrl *)((u8 *)ptclPtbData + ptclPtbData->ctrlOffset) + arg1;
    slot = PtclStarterSlotAlloc(rec);
    ptclStarterInst[arg0] = slot;
    if (slot == NULL) {
        return;
    }
    req.ctrl = rec;
    c = *PtclPtbGetString(ptclPtbData, rec->resIndex);
    if (c == 1 || c == 3) {
        req.texId = ptclTexIdTable[rec->resIndex];
        req.model = NULL;
        req.animSet = (s32)ptclAnimSetTable[rec->resIndex];
    } else {
        req.texId = 0;
        req.model = ptclModelTable[rec->resIndex];
    }
    req.partPrio = 10;
    /* one argument: PtclGenCalcSize's arg1 is left as it is (see 44A660.c) */
    h = MemAlloc16C(((s32 (*)(PtclGenRequest *))PtclGenCalcSize)(&req));
    slot->gen = (PtclGen *)h;
    if (PtclGenCreate(h, &req) != 0) {
        slot->time = 0;
        if (arg2 != NULL) {
            PtclGen *o = slot->gen;
            o->pos[0] = arg2->pos[0];
            o->rot[0] = arg2->rot[0];
            o->pos[1] = arg2->pos[1] + ptclStartOffsetY;
            o->rot[1] = arg2->rot[1];
            o->pos[2] = arg2->pos[2];
            o->rot[2] = arg2->rot[2];
        }
        size = ((s32 (*)(PtclGenRequest *))PtclGenCalcSize)(&req);
        ptclCurWork += size;
        ptclStarterWorkSize[arg0] = size;
        if (ptclPeakWork < ptclCurWork) {
            ptclPeakWork = ptclCurWork;
        }
    } else {
        if (slot->gen != NULL) {
            MemFree((s32)slot->gen);
        }
        PtclStarterSlotFree((s32 *)slot);
        ptclStarterInst[arg0] = NULL;
    }
}
