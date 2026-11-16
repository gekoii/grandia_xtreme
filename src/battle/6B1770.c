#include "common.h"
#include "protos/battle.h"
#include "ptcl.h"
#include "battle/misc.h"
#include "battle/n31.h"

void BtlPtclGenReqFill(PtclGenRequest *, PtclPtbHeader *, s32, void *);

void MgcPtclGroupsUpdate(void) {
    PtclGenRequest req;
    MgcWork *m = &mgcWork;
    s32 i;
    s32 j;
    MgcPtclGroup *g;
    PtclStarterEntry *e;
    PtclStarterSlot *o;
    PtclGen *p;
    PtclPtbHeader *h;
    s16 idx;

    for (i = 0; i < 256; i++) {
        g = m->ptclGroups[i];
        if (g == NULL) {
            continue;
        }
        e = (PtclStarterEntry *)((u8 *)g->ptb + g->ptb->starterOffset);
        for (j = 0; j < g->count; j++, e++) {
            if (g->slots[j] == NULL) {
                if (e->time == g->frame && e->ctrl >= 0) {
                    if ((m->unk13 & 6) ? e->id != 2 : e->id != 1) {
                        idx = e->ctrl;
                        h = g->ptb;
                        BtlPtclGenReqFill(&req, h, idx, m->resources[((PtclCtrl *)((u8 *)h + h->ctrlOffset))[idx].resIndex]);
                        o = PtclStarterSlotAlloc((s32)req.ctrl);
                        g->slots[j] = o;
                        if (o == NULL) {
                            continue;
                        }
                        o->gen = (PtclGen *)MemAlloc16C(((s32 (*)(PtclGenRequest *))PtclGenCalcSize)(&req));
                        if (PtclGenCreate((s32)o->gen, &req) == 0) {
                            if (o->gen != NULL) {
                                MemFree(o->gen);
                            }
                            PtclStarterSlotFree((s32 *)o);
                            g->slots[j] = NULL;
                        }
                    }
                }
                if (g->slots[j] == NULL) {
                    continue;
                }
            }
            p = g->slots[j]->gen;
            p->pos[0] = g->pos[0] + e->pos[0];
            p->rot[0] = g->rot[0] + e->rot[0];
            p->pos[1] = g->pos[1] + e->pos[1];
            p->rot[1] = g->rot[1] + e->rot[1];
            p->pos[2] = g->pos[2] + e->pos[2];
            p->rot[2] = g->rot[2] + e->rot[2];
        }
        g->frame++;
    }
    MgcPtclGroupsAreDone();
}
