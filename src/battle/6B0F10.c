/* cflags: -fno-strength-reduce */
/* -fno-strength-reduce: the comboPartners loop indexes (with it on, it walks a pointer) */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"


void MgcStart(s32 arg0, BtlUnit *arg1, s32 arg2) {
    MgcWork *m = &mgcWork;
    s32 n;
    s32 i;
    s32 k;
    s32 j1, j2, j3, j4;
    s32 *p;
    s32 *p2;
    s32 *q2;
    s32 *q3;
    BtlUnit *c;

    if (arg0 <= 0) {
        arg0 = 1;
    }
    n = arg0;
    m->state = 1;
    m->part = 1;
    m->id = n;
    m->phase = 0;
    m->count = 0;
    m->motionTime = 0;
    m->unk14 = (rand() >> 15) & 1;
    if (btlDebugCfg.se >= 0) {
        m->unk14 = btlDebugCfg.se;
    }
    m->caster = arg1;
    if (arg1 == NULL) {
        m->unk13 = 0;
        m->hitData = 0;
    } else {
        if (arg1->id < 4 && arg1->action == 3) {
            i = m->part - 1;
            m->unk13 = ptyPartyData->eggs[arg1->eggNo[i]].spellUpgrades[arg1->eggMagic[i]];
        } else {
            m->unk13 = 0;
        }
        MgcSelectHitData(0);
    }
    m->nextPart = 0;
    m->partStart = 0;
    m->draw = MgcProcNop;
    m->model = 0;
    m->update = 0;
    m->drawList = 0;
    m->textures = MemAlloc32(GphTexTableSize(0x20));
    GphTexTableInit(0x20, m->textures);
    m->ptclSystem = MemAlloc16C(PtclSystemCalcSize(0x40, 8, 0x40));
    PtclSystemInit(m->ptclSystem, 0x40, 8, 0x40);
    mgcPtclModelCount = 0;
    p = &m->ptclGroups[255];
    for (j1 = 255; j1 >= 0; j1--) {
        *p = 0;
        p--;
    }
    q3 = m->modelData; /* dead early loads: they reorder PRE (register choice) */
    p2 = m->models;
    q2 = m->lightIds;
    for (j2 = 8; j2 > 0; j2--) {
        *q2 = -1;
        q2++;
    }
    p2 = m->models;
    q3 = m->modelData;
    for (j3 = 255; j3 >= 0; j3--) {
        *q3 = 0;
        *p2 = 0;
        q3++;
        p2++;
    }
    if (arg1 != NULL && arg1->comboPartners[0] >= 0) {
        j4 = 0;
        do {
            k = arg1->comboPartners[j4];
            if (k >= 0) {
                c = &btlUnits[k];
                c->target = arg1->target;
                c->flags |= 8;
            }
            j4++;
        } while (j4 < 3);
    }
    MgcLoadFile((btlDebugCfg.flags & 0x40) ? 0x10 : 0x11, arg2);
    btlHitUnitMask = 0;
    mgcSeLoadBusy = 0;
    if (n == 0x46) {
        BtlUnitSetReviveHpMode(0, 1);
    } else {
        BtlUnitSetReviveHpMode(0, 0);
    }
}
