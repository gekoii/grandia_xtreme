/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/misc.h"
#include "party.h"
#include "gph.h"


void BtlUnitsInit(void) {
    BtlUnit *u;
    BtlUnit *base;
    BtlUnit *l;
    BtlCharDef *info;
    GphModel *sh;
    BtlUnit *x;
    BtlUnit *w;
    void *p;
    s32 i;
    s32 n;
    s32 j;

    btlActingUnit = 0xFF;
    btlKoRequestMask = 0;
    btlActRequestMask = 0;
    btlActDoneMask = 0;
    btlMotionSeMuteFirst = btlMotionSeMuteLast = 0;
    BtlUnitsSetup();
    btlInvPartyAvgSpeed = 0.0f;
    n = 0;
    u = btlUnits;
    for (i = 0; i < 4; i++, u++) {
        if (u->state > 0) {
            n++;
            btlInvPartyAvgSpeed += u->params->params[4];
        }
    }
    btlInvPartyAvgSpeed = 1.0f / (btlInvPartyAvgSpeed / n);
    x = btlUnits;
    BtlUnitsCreateModels(&btlPartyDrawList, x, 4);
    BtlUnitsCreateModels(&btlEnemyDrawList, x + 4, 12);
    ChrModelPoolSet(NULL);
    u = x;
    base = u;
    for (i = 15; i >= 0; i--, u++) {
        info = u->def;
        if (info == NULL) {
            continue;
        }
        if (u->body >= 0) {
            l = (BtlUnit *)((u->body << 9) + (u32)base); /* integer index: addu order (scaled index first) */
            u->params->bitCsf |= 0x1600 | (l->params->bitCsf & 4);
            u->rotVel = l->rotVel;
            u->rotAccel = l->rotAccel;
            u->rotVelMax = l->rotVelMax;
        }
        sh = u->model;
        if (sh != NULL) {
            p = (u8 *)&u->face;
            sh->flags |= 0x200;
            ChrFaceAnimInit(p, info->faceData, sh);
            ChrFaceAnimSet(p, 0, 1);
        }
        if (u->id >= 4 && u->def->data->unkDC != 0) {
            u->params->bitCsf |= 0x500;
        }
        if (u->def->init != NULL) {
            u->def->init(u);
        }
    }
    w = btlUnits;
    for (j = 3; j >= 0; ) {
        j--;
        if (w->state >= 4) {
            BtlUnitWeaponFxStart(w);
        }
        w++;
        if (j < 0) break;
    }
}
