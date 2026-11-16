/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "gph.h"
#include "field/state.h"
#include "field/n22.h"

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")


void FldSpotUpdate(void) {
    ChrObj obj;
    FldMapSet *ev;
    ChrObj *pc;
    FldSpot *e;
    FldSpot *e2;
    FldSpotDef *a;
    GphModel *mot;
    ChrObj *c;
    GphModel *m;
    u32 t;
    s32 found;
    u32 i;
    s32 k;

    ev = MapSetFileGet();
    if (ev == NULL) {
        return;
    }
    pc = ChrGetByNo(1);
    found = -1;
    i = 0;
    while (i < fldSpotWork->count) {
        if (fldSpotWork->list[i].unkB != 0) {
            found = i;
            break;
        }
        i++;
    }
    if (found == -1) {
        return;
    }
    t = found * sizeof(FldSpot);
    e = (FldSpot *)(t + (u32)fldSpotWork->list);
    if (e->unkE != 0) {
        e->unkE--;
        return;
    }
    a = (FldSpotDef *)((e->unk9 << 5) + (u32)fldSpotWork->hdr->entries);
    mot = fldSpotWork->models[a->inst];
    k = a->motion2;
    do { /* empty: without it two saved registers (s1/s2) swap */
    } while (0);
    if (k == -1) {
        k = a->motion;
    }
    if (mot == NULL) {
        return;
    }
    obj.model = mot;
    obj.motTable = &fldSpotWork->motTable;
    switch (fldSpotWork->list[found].unkB) {
    case 1:
        FldSndSePlay(3, 0xFA2, 0);
        t = (u32)&mot->pos;
        mot->flags ^= 8;
        fldSpotWork->list[found].model->flags |= 8;
        copyq(mot->pos, fldSpotWork->list[found].model->pos);
        copyq(mot->scale, fldSpotWork->list[found].model->scale);
        mot->rot[0] = fldSpotWork->list[found].model->rot[0];
        mot->rot[1] = fldSpotWork->list[found].model->rot[1];
        mot->rot[2] = fldSpotWork->list[found].model->rot[2];
        ChrModelSetAlpha(mot, 0.0f);
        ChrMotSetOnce(&obj, k);
        ChrMotSetFrame(mot, 0.0f);
        EffSpawnSpotOpen((void *)t);
        if (EvtGetMapNo() == 0x5010) {
            EffSpawnCode5A((void *)t);
        }
        if (fldSpotWork->list[found].kind == 3) {
            EffSpawnSpotKind3((void *)t);
            ChrHurtStart(pc);
            FldTrapDamageApply(&pc->hpGauge, ev->trapDamage);
            FldSndSePlay(3, 0x7E3, 0);
            FldSndSePlay(3, VuRandInt(2) + 0xFB0, 0);
        } else {
            FldSndSePlay(3, VuRandInt(3) + 0x79E, 0);
        }
        fldSpotWork->list[found].unkB++;
        break;
    case 2:
        if (ChrMotIsEnd(mot) != 1) {
            break;
        }
        fldSpotWork->list[found].unkB++;
        switch (fldSpotWork->list[found].kind) {
        case 1:
            FldSndSePlay(3, 0x7DD, 0);
            EffSpawnPickup(fldSpotWork->list[found].model->pos);
            break;
        case 2:
            EffSpawnChara(fldSpotWork->list[found].chara->model->pos, 1);
            FldSndSePlay(3, 0x7DD, 0);
            break;
        }
        break;
    case 3:
        ChrModelAddAlpha(mot, -4.0f);
        if (ChrModelGetAlpha(mot) > -127.0f) {
            break;
        }
        mot->flags |= 8;
        e2 = (FldSpot *)(found * sizeof(FldSpot) + (u32)fldSpotWork->list);
        switch (e2->kind) {
        case 1:
            btlHandoff.dropCount = 1;
            btlHandoff.unkC = 1;
            btlHandoff.dropIds[0] = (s16)fldSpotWork->itemIds[found];
            copyq(&btlHandoff.pos, e2->model->pos);
            fldSpotWork->list[found].unkB = 0;
            fldSpotWork->list[found].unkA = 0;
            break;
        case 4:
            if (FldPartyHealFull(&pc->hpGauge) != 0) {
                FldSndSePlay(3, 0x7D8, 0);
                EffSpawnAtPlayer(pc->model->pos, 0);
            }
            fldSpotWork->list[found].unkB = 0;
            fldSpotWork->list[found].unkA = 0;
            break;
        case 2:
            c = e2->chara;
            c->actFlags |= 0x400;
            ChrTenantSetActive(c, 1);
            ChrTurnToPlayer(fldSpotWork->list[found].chara);
            m = fldSpotWork->list[found].chara->model;
            m->scale[0] -= 0.08f;
            m->scale[1] += 0.08f;
            m->scale[2] = m->scale[0];
            if (m->scale[0] < 1.0f) {
                fldSpotWork->list[found].chara->actFlags ^= 0x400;
                m->scale[0] = 1.0f;
                m->scale[1] = 1.0f;
                m->scale[2] = 1.0f;
                fldSpotWork->list[found].unkB = 0;
                fldSpotWork->list[found].unkA = 0;
                fldSpotWork->list[found].chara->flags |= 0x20;
            }
            break;
        default:
            fldSpotWork->list[found].unkB = 0;
            fldSpotWork->list[found].unkA = 0;
            break;
        }
        break;
    }
}
