#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"

/* Track k of the current area: words 2-5 of ChrObj.tracks[area]. */
#define TRACK(a, k) ((a)->tracks[(a)->area][(k) + 2])

extern ChrTenantOpFunc chrTenantOpTable[];
extern s16 chrTenantOpChainFlags[];

void ChrTenantAiUpdate(ChrObj *a, ChrTenantTrack *t) {
    s32 r;
    s16 op;
    s16 p1;
    s16 p2;
    ChrEnemyWork *s;

    if (t == NULL) {
        return;
    }
    s = a->sense;
    if (s != NULL && (a->aiMode == 1 || a->aiMode == 3)) {
        if (--s->seTimer <= 0) {
            ChrTenantPlaySe(s->charDef->se, (s32)a->model->pos);
            CHR_SENSE(a)->seTimer = CHR_SENSE(a)->charDef->seInterval + VuRandInt(CHR_SENSE(a)->charDef->seIntervalRand);
        }
    }
    if (a->aiMode != 0 && a->collMask == 0) {
        ChrTenantAiSetMode(a, 3);
        t = TRACK(a, 1);
    }
    if (a->sense != NULL && ChrTenantCanSeeLeader(a, a->sense) != 0) {
        switch (a->aiMode) {
        case 4:
        case 5:
            ChrTenantAiSetMode(a, 2);
            t = TRACK(a, 0);
            break;
        case 0:
            ChrTenantAiSetMode(a, 4);
            t = TRACK(a, 2);
            break;
        case 1:
            ChrTenantAiSetMode(a, 5);
            t = TRACK(a, 3);
            break;
        }
    }
    for (;;) {
        op = t->ops[a->opPc].op;
        p1 = t->ops[a->opPc].arg1;
        p2 = t->ops[a->opPc].arg2;
        if (op >= 0x1E) {
            return;
        }
        if (op < 0) {
            return;
        }
        if (chrTenantOpTable[op] == NULL) {
            return;
        }
        if (a->sense != NULL) {
            if (op == 3) {
                p2++;
            }
            if (op == 6) {
                p2++;
            }
            if (op == 7) {
                p1++;
            }
            if (op == 9) {
                p1++;
            }
            if (op == 0xB) {
                p1++;
            }
            if (op == 0xC) {
                p1++;
            }
        }
        r = chrTenantOpTable[op](t, a, p1, p2);
        if (ChrTenantCheckLeaveArea(a, t) != 0) {
            return;
        }
        if (a->opState == 0) {
            a->opTimer = 0;
        }
        if (r != 0) {
            break;
        }
        a->opState = 0;
        if (++a->opPc >= 0x20) {
            if (a->aiMode == 0 || a->aiMode == 3) {
                a->aiMode = 1;
                a->opPc = 0;
                if (a->sense != NULL) {
                    CHR_SENSE(a)->flags |= 4;
                    CHR_SENSE(a)->seTimer = CHR_SENSE(a)->charDef->seInterval + VuRandInt(CHR_SENSE(a)->charDef->seIntervalRand);
                }
                break;
            }
            if (a->aiMode == 4 || a->aiMode == 5) {
                a->opPc = 0;
                a->aiMode = 2;
                break;
            }
            if (a->aiMode == 1 || a->aiMode == 2) {
                a->aiMode = 3;
                a->opPc = 0;
                break;
            }
        }
        op = t->ops[a->opPc].op;
        if (chrTenantOpChainFlags[op] != 1) {
            break;
        }
    }
}
