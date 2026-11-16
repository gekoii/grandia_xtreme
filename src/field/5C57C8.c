#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "party.h"
#include "gph.h"
#include "field/n14.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void FldTrapDamageApply(FldHpGauge *t, s32 dmg) {
    s32 i;
    ChrObj *o;
    s32 off;

    t->cur -= dmg;
    if (t->cur <= 0) {
        t->cur = 1;
    }
    t->flag = 0;
    t->animT = 0;
    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] != 0 && !(ptyPartyUnits[i].status & 0x8000)) {
            ptyPartyUnits[i].hp -= dmg;
            if (ptyPartyUnits[i].hp <= 0) {
                ptyPartyUnits[i].hp = 1;
            }
        }
    }
    o = ChrGetByNo(1);
    fldDmgPopWork->active[fldDmgPopWork->next] = 1;
    fldDmgPopWork->value[fldDmgPopWork->next] = dmg;
    fldDmgPopWork->unk4400[fldDmgPopWork->next] = 0;
    fldDmgPopWork->t[fldDmgPopWork->next] = 0;
    off = (fldDmgPopWork->next << 4) + 0x4140;
    copyq((u8 *)fldDmgPopWork + off, &o->model->pos);
    fldDmgPopWork->pos[fldDmgPopWork->next][0] += VuRandSigned(64.0f);
    fldDmgPopWork->pos[fldDmgPopWork->next][1] += VuRandSigned(32.0f);
    fldDmgPopWork->pos[fldDmgPopWork->next][2] += VuRandSigned(64.0f);
    fldDmgPopWork->next++;
    if (fldDmgPopWork->next >= 32) {
        fldDmgPopWork->next = 0;
    }
}
