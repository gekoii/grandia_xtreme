/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n32.h"

void VuVecCopy(f32 *, f32 *);

s32 BtlUnitMotionEvents(BtlUnit *u, s32 n) {
    FVECTOR v;
    s16 *p;
    s16 *start;
    BtlUnit *q;
    PtyMoveEffect *inf;
    s16 c;
    s32 j;
    s32 mode;
    f32 f;

    p = u->evtCur;
    start = p;
    while (n < 4 && u->evtCur != NULL) {
        if (*p < 0) {
            u->evtCur = NULL;
        }
        if (u->evtCur == NULL) {
            break;
        }
        {
        s32 k;
        s32 k2;
        s32 k3;
        s32 k4;
        if (!(*p <= u->frame)) {
            goto out; /* goto: a break here reorders the whole loop */
        }
        p++;
        if (*p >= 0x400) {
            switch (*p) {
            case 0x400:
                ChrPartDispSetOne(u, p[3], p[2]);
                break;
            case 0x409:
                ChrFaceAnimSet(&u->face, p[2], p[3]);
                break;
            case 0x401:
                BtlCamQuakeStart(p[2]);
                break;
            case 0x407:
                if (u->loopCount == 0) {
                    u->loopStart = p[-1];
                    u->evtLoop = start;
                    u->loopCount = p[2];
                }
                break;
            case 0x408:
                c = u->loopCount;
                if (c < 0) {
                    u->loopEnd = f = u->motionLast;
                    u->evtLoop = NULL;
                    u->loopStart = f;
                } else if (c == 1) {
                    if (u->motionNo == 4) {
                        u->loopEnd = f = u->motionLast;
                        u->evtLoop = NULL;
                        u->loopStart = f;
                    } else {
                        f = u->motionLast;
                        u->loopEnd = f;
                        u->evtLoop = u->evtStart;
                        u->loopStart = 0.0f;
                    }
                } else {
                    u->loopEnd = p[-1];
                }
                if (u->loopCount > 0) {
                    u->loopCount--;
                }
                break;
            case 0x402:
                if (p[3] > 0) {
                    BtlChrPartToWorld(v, u, BtlChrFindPart(u->model, p[3]), 0);
                } else {
                    VuVecCopy(v, u->pos);
                }
                k2 = p[2];
                if (k2 < 3) {
                    if (k2 >= 0) {
                        PtclBtlDustSpawn(k2 + 0x80, NULL, v[0], v[2]);
                    }
                }
                break;
            case 0x403:
                if (p[2] == 1) {
                    u->flags |= 2;
                } else {
                    u->flags &= 0xFFFFFFFD;
                }
                break;
            case 0x404:
                k = (s8)p[3];
                if (k == 0) {
                    if (u->partsChr >= 0) {
                        q = &btlUnits[u->partsChr];
                    } else {
                        q = u;
                    }
                    if (q->id < 4) {
                        if (q->action >= 2 && q->state >= 0xC0) {
                            k = MgcGetHitData()->element;
                        } else {
                            k = q->params->eqpAttr;
                        }
                    } else {
                        if (q->state < 0xC0) {
                            inf = q->def->data->actionSets[0].actions[0]->level->effects;
                        } else if (q->skillId == 0) {
                            inf = q->def->data->actionSets[q->ai].actions[q->action]->level->effects;
                        } else {
                            inf = MgcGetHitData();
                        }
                        k = inf->element;
                    }
                } else if (k == 9) {
                    k = 0;
                }
                j = p[2];
                if (btlActingUnit != 0xFF || btlPhase != 0x80) {
                    mode = 8;
                } else {
                    mode = 2;
                }
                FxTraceStart(u, j, 8, mode, k);
                break;
            case 0x405:
                if (p[3] <= 0) {
                    FxTraceStop(u->id, p[2]);
                } else {
                    FxTraceSetFadeOut(u->id, 0x20);
                }
                break;
            case 0x406:
                if (p[2] != 0) {
                    u->flags |= 0x100;
                } else {
                    u->flags &= 0xFFFFFEFF;
                }
                break;
            case 0x7FFF:
                k3 = p[2];
                if (k3 < btlMotionSeMuteFirst || btlMotionSeMuteLast < k3) {
                    SndSePlay3D(k3, 2, 8, &u->pos);
                }
                break;
            case 0x7FFE:
                k4 = p[2];
                if (k4 < btlMotionSeMuteFirst || btlMotionSeMuteLast < k4) {
                    SndSePlay(k4, 2, 0, p[3], p[4]);
                }
                break;
            }
            p++;
        } else {
            u->evtIds[n] = *p;
            p++;
            u->evtArgs[n] = p;
            n++;
        }
        p += *p + 1;
        u->evtCur = p;
        }
    }
out:
    u->evtIds[n] = 0;
    return n;
}
