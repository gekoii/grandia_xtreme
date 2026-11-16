/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "gph.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n12.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

void EvtAreaCalcMatrix(EvtArea *);

void EvtAreaUpdate(Pad *pad) {
    s32 n;
    s32 i;
    EvtArea *e;
    ChrObj *pl;
    ChrObj *o;
    GphModel *q;
    s32 trig;
    s32 hit;
    s32 old;
    s32 diff;
    s32 on;
    s32 off;
    s32 j;

    if (evtAreaUse == 0) {
        return;
    }
    n = evtAreaTable->count;
    evtAreaFrameWorkA = 0;
    evtAreaFrameWorkB = 0;
    evtAreaStepPriority = 0;
    evtAreaStepSound = 0;
    pl = ChrGetByNo(1);
    if (pl == NULL) {
        return;
    }
    trig = 0;
    pl->actFlags |= 4;
    if (ChrPlayerIsBusy() == 0 && camCtl.aiType != 2 && !(pad->held & 1)) {
        trig = pad->repeat & 0x20;
    }
    for (i = 0; i < n; i++) {
        e = (EvtArea *)((u8 *)evtAreaTable->areas + (i << 8));
        if (!(e->flags & EVT_AREA_ENABLED)) {
            e->result = 0;
            continue;
        }
        if ((e->result & 1) && (e->flags & 2)) {
            e->result = 0;
            e->flags &= 0xFE;
            continue;
        }
        e->result = 0;
        if (e->flags & 4) {
            EvtAreaCalcMatrix(e);
        }
        if (e->flags & EVT_AREA_SCALED) {
            e->unk5C = MAX(MAX(e->scale[0], e->scale[1]), e->scale[2]);
        }
        switch (e->target) {
        case 0:
        case 3:
        case 5:
            if (e->mode == 0 && trig == 0) {
                e->result = 0;
                continue;
            }
            hit = 0;
            if (e->chk & 1) {
                pl = ChrGetByNo(1);
                if (pl == NULL) {
                    continue;
                }
                if (!(pl->flags & CHR_FLAG_DRAWN)) {
                    continue;
                }
                hit = EvtAreaHitTest(pl->model->pos, 40.0f, e) != 0;
            }
            if (e->chk & 4) {
                for (j = (s16)e->start; j < (s16)e->start + e->count; j++) {
                    o = ChrTenantAt(j);
                    if (o == NULL) {
                        break;
                    }
                    if (!(o->flags & CHR_FLAG_ENCOUNTER)) {
                        continue;
                    }
                    if (EvtAreaHitTest(o->model->pos, o->radius, e) != 0) {
                        hit |= 1;
                        break;
                    }
                }
            }
            if (e->chk & 2) {
                for (j = (s16)e->start; j < (s16)e->start + e->count; j++) {
                    o = ChrTenantAt(j);
                    if (o == NULL) {
                        break;
                    }
                    if (o->flags & CHR_FLAG_ENCOUNTER) {
                        continue;
                    }
                    if (EvtAreaHitTest(o->model->pos, o->radius, e) != 0) {
                        hit |= 1;
                        break;
                    }
                }
            }
            if (e->chk & 8) {
                for (j = (s16)e->start; j < (s16)e->start + e->count; j++) {
                    q = FldObjGet(j);
                    if (q == NULL) {
                        break;
                    }
                    if (EvtAreaHitTest(q->pos, 0.0f, e) != 0) {
                        hit |= 1;
                        break;
                    }
                }
            }
            if (e->chk & 0x10) {
                if (EvtAreaHitTest(camField[0].eye, 40.0f, e) != 0) {
                    hit |= 1;
                }
            }
            old = e->state;
            e->state = hit;
            diff = hit ^ old;
            on = hit & diff;
            off = old & diff;
            switch (e->mode) {
            case 0:
                if (hit & 1) {
                    e->result |= 1;
                }
                e->result |= (hit & 0xF) << 4;
                break;
            case 1:
                if (on & 1) {
                    e->result |= 3;
                } else {
                    e->result |= 2;
                }
                e->result |= (on & 0xF) << 4;
                break;
            case 2:
                if (off & 1) {
                    e->result |= 5;
                } else {
                    e->result |= 4;
                }
                e->result |= (off & 0xF) << 4;
                break;
            case 3:
                if (hit) {
                    e->result |= 7;
                } else {
                    e->result |= 6;
                }
                e->result |= (hit & 0xF) << 4;
                break;
            case 4:
                if (hit & 1) {
                    e->result |= 9;
                } else {
                    e->result |= 8;
                }
                e->result |= (hit & 0xF) << 4;
                break;
            }
            if (e->target == 3 && (e->result & 0x11) == 0x11 && (s16)e->start >= evtAreaStepPriority) {
                evtAreaStepPriority = (s16)e->start;
                evtAreaStepSound = e->count;
            }
            break;
        case 1:
            if (EvtAreaHitTest(pl->model->pos, 40.0f, e) == 0) {
                break;
            }
            if (pl->actFlags & 4) {
                pl->actFlags ^= 4;
            }
            break;
        case 2:
        case 4:
        default:
            break;
        }
    }
}
