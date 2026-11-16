#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/event.h"


void OccsUpdate(void)
{
    s32 i;
    s32 j;
    s32 idx;
    s32 a;
    s32 found;
    OccsEntry *it;
    ChrObj *e;
    EvtArea *o;
    OccsEntry *p;

    for (i = 0; i < occsTable->count; i++) {
        it = &occsTable->items[i];
        if (it->flags & 0x10) {
            switch (it->state) {
            case 0:
                break;
            case 1:
                if (OccCondCheck(&it->cond) == 0) {
                    it->state = 0;
                    it->flags &= ~0x10;
                    break;
                }
                it->state = 2;
                it->flags &= ~0x20;
            case 2:
                idx = it->idx;
                while (idx < 32) {
                    if (it->cmds[idx].op == 1) {
                        if (it->cmds[idx].arg < ++it->timer) {
                            idx++;
                            it->timer = 0;
                        }
                        break;
                    } else if (it->cmds[idx].op == 2) {
                        if (OccIsActive(it->cmds[idx].arg) != 0) {
                            break;
                        }
                        idx++;
                    } else if (it->cmds[idx].op == 3) {
                        a = it->cmds[idx].arg;
                        if (it->cmds[a].op != 0x101) {
                            idx++;
                        } else {
                            if (OccIsActive(it->cmds[a].arg) != 0) {
                                break;
                            }
                            idx++;
                        }
                    } else if (it->cmds[idx].op == 4) {
                        e = ChrGetByNo(1);
                        o = EvtAreaGet(it->areaId);
                        if (e != NULL && o != NULL) {
                            e->heading = (FldCalcYaw(e->model->pos, o->pos) - 0x8000) & 0xFFFF;
                            __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(e->vel) : "memory");
                        }
                        idx++;
                    } else if (it->cmds[idx].op == 0x101) {
                        OccWake(it->cmds[idx].arg);
                        idx++;
                    } else if (it->cmds[idx].op == 0x102) {
                        OccsWake(it->cmds[idx].arg);
                        idx++;
                    } else {
                        idx++;
                    }
                }
                if (idx >= 32) {
                    if ((it->flags & 1) && !(it->flags & 0x20)) {
                        it->idx = 0;
                        it->timer = 0;
                    } else {
                        it->timer = 0;
                        it->state = 0;
                        it->idx = 0;
                        it->flags &= ~0x10;
                    }
                } else {
                    it->idx = idx;
                }
                break;
            }
        } else {
            if ((it->flags & 0x82) == 2 && OccCondCheck(&it->cond) == 1) {
                it->flags |= 0x80;
                OccsWake(i);
            }
            a = EvtAreaGetResult(it->areaId);
            found = 0;
            if (a & 1) {
                if (it->flags & 4) {
                    for (j = 0; j < occsTable->count; j++) {
                        p = &occsTable->items[j];
                        if ((p->flags & 0x14) == 0x14 && i != j) {
                            found = 1;
                            break;
                        }
                    }
                }
                if (!found) {
                    OccsWake(i);
                } else {
                    EvtAreaGet(it->areaId)->result &= ~1;
                }
            }
        }
    }
}
