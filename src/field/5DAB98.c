#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n16.h"
#include "field/occ.h"

extern s32 dbgFldOccNo;
extern char dbgOccStrNumber[];
extern char dbgOccStrCat[];
extern char dbgOccStrEnabledOn[];
extern char dbgOccStrEnabledOff[];
extern char dbgOccStrDisabledOn[];
extern char dbgOccStrDisabledOff[];
extern char dbgOccStrType[];
extern char dbgOccStrFirstWait[];
extern char dbgOccStrBeginWait[];
extern char dbgOccStrTargetId[];
extern char dbgOccStrMethod[];
extern char dbgOccStrProgress[];
extern char dbgOccStrFwBeginVec[];
extern char dbgOccStrVwDeltaVec[];
extern char dbgOccStrVwProgressPair[];
extern char dbgOccStrFwDecay[];
extern char dbgOccStrVwDir[];
extern char dbgOccStrAlpha[];
extern char dbgOccStrCountPair[];
extern char dbgOccStrHalfWait[];
extern char dbgOccStrEndWait[];
extern char dbgOccStrFwBegin[];
extern char dbgOccStrFwEnd[];
extern char dbgOccStrVwStatus[];
extern char dbgOccStrVwProgress[];
extern char dbgOccStrVwDelta[];

#define PRINTF_NT ((s32 (*)(s32, s32, char *, ...))DbgPrintf)

void DbgFldOccPage(void) {
    Pad *pad;
    OccEntry *e;
    s32 cur;
    s32 lv;
    s32 prev;
    s32 n;
    s32 i;

    pad = &padPort1;
    if (!(padPort1.held & 2)) {
        if (pad->repeatFast & 0x1000) {
            cur = OCC_ID_GRP(OccGet(dbgFldOccNo)->id);
            for (i = dbgFldOccNo; i < OccGetCount() - 1; i++) {
                if (cur < OCC_ID_GRP(OccGet(i)->id)) {
                    dbgFldOccNo = i;
                    break;
                }
            }
        }
        if (pad->repeatFast & 0x4000) {
            prev = -1;
            cur = OCC_ID_GRP(OccGet(dbgFldOccNo)->id);
            n = dbgFldOccNo;
            for (i = 0; i < n; i++) {
                lv = OCC_ID_GRP(OccGet(i)->id);
                if (lv < cur && prev != lv) {
                    dbgFldOccNo = i;
                    prev = lv;
                }
            }
        }
        if ((pad->repeatFast & 0x8000) && dbgFldOccNo != 0) {
            dbgFldOccNo--;
        }
        if ((pad->repeatFast & 0x2000) && dbgFldOccNo < OccGetCount() - 1) {
            dbgFldOccNo++;
        }
        if (pad->trigger & 0x20) {
            OccWake(dbgFldOccNo);
        }
    }
    e = OccGet(dbgFldOccNo);
    DbgPrintf(2, 4, dbgOccStrNumber, dbgFldOccNo, OccGetCount());
    if (e != NULL) {
        DbgPrintf(2, 3, dbgOccStrCat, OCC_ID_CAT(e->id));
        DbgPrintf(14, 3, dbgOccStrGrp, OCC_ID_GRP(e->id));
        DbgPrintf(24, 3, dbgOccStrIndex, OCC_ID_IDX(e->id));
        if (e->flags & 0x10) {
            DbgPrintf(2, 5, dbgOccStrEnabledOn);
        } else {
            DbgPrintf(2, 5, dbgOccStrEnabledOff);
        }
        if (e->flags & 0x20) {
            DbgPrintf(2, 6, dbgOccStrDisabledOn);
        } else {
            DbgPrintf(2, 6, dbgOccStrDisabledOff);
        }
        DbgPrintf(2, 7, dbgOccStrType, e->kind);
        DbgPrintf(2, 8, dbgOccStrFirstWait, e->unk1A);
        DbgPrintf(2, 9, dbgOccStrBeginWait, e->unk1C);
        DbgPrintf(24, 4, dbgOccStrStatus, dbgFldOccStatusNames[e->state]);
        DbgPrintf(24, 5, dbgOccStrCount, e->timer);
        if (e->flags & 1) {
            DbgPrintf(24, 6, dbgOccStrLoopOn);
        } else {
            DbgPrintf(24, 6, dbgOccStrLoopOff);
        }
        if (e->flags & 2) {
            DbgPrintf(24, 7, dbgOccStrAutoWakeOn);
        } else {
            DbgPrintf(24, 7, dbgOccStrAutoWakeOff);
        }
        DbgPrintf(24, 8, dbgOccStrAttribute, (s16)e->flags);
        switch (e->kind) {
        case 2:
        case 3:
        case 4: {
            OccTween *a = &e->params.tween;
            DbgPrintf(2, 12, dbgOccStrTargetId, a->id);
            DbgPrintf(2, 13, dbgOccStrMethod, a->ease);
            DbgPrintf(2, 14, dbgOccStrProgress, a->count);
            DbgPrintf(2, 15, dbgOccStrFwBeginVec, a->from[0], a->from[1], a->from[2]);
            PRINTF_NT(2, 16, dbgOccStrVwDeltaVec, a->delta.f[0], a->delta.f[1], a->delta.f[2]);
            break;
        }
        case 11: {
            OccPartPair *b = &e->params.partPair;
            PRINTF_NT(2, 12, dbgOccStrVwProgressPair, b->done[0], b->done[1]);
            break;
        }
        case 15: {
            OccShake *c = &e->params.shake;
            DbgPrintf(2, 12, dbgOccStrFwDecay, c->decay);
            PRINTF_NT(2, 13, dbgOccStrVwDir, c->dir[0], c->dir[1], c->dir[2]);
            break;
        }
        case 29: {
            OccAlphaPulse *d = &e->params.alphaPulse;
            DbgPrintf(2, 12, dbgOccStrAlpha, d->alpha, (s16)d->alphaEnd);
            DbgPrintf(2, 13, dbgOccStrCountPair, d->timeIn, d->timeOut);
            DbgPrintf(2, 14, dbgOccStrHalfWait, d->hold);
            DbgPrintf(2, 15, dbgOccStrEndWait, (s16)d->backTime);
            DbgPrintf(2, 16, dbgOccStrFwBegin, d->start[0]);
            DbgPrintf(2, 17, dbgOccStrFwEnd, d->end[0]);
            DbgPrintf(2, 18, dbgOccStrVwStatus, d->phase);
            DbgPrintf(2, 19, dbgOccStrVwProgress, d->frame);
            PRINTF_NT(2, 20, dbgOccStrVwDelta, d->delta[0]);
            break;
        }
        }
    }
}
