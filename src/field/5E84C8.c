#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "save.h"
#include "field/state.h"
#include "field/n17.h"

extern ChrObj *chrObjPool;

void *FldMemAlloc(s32);

void ChrPartyObjCreate(s32 arg0, s32 arg1, s32 arg2) {
    GphPart *n;

    if (arg0 != 0) {
        ChrObjClear(&chrObjPool[arg1]);
        chrPartyModels[arg1] = ChrModelCreate(FilePacGetModel(arg0, 0));
        ChrModelSetPartPrio(chrPartyModels[arg1], 2);
        ChrObjSetModel(&chrObjPool[arg1], chrPartyModels[arg1]);
        EffSpawnModelEffects(chrPartyModels[arg1]);
        ChrRegister(arg1 + 1, &chrObjPool[arg1]);
        chrObjPool[arg1].trail = FldMemAlloc(0x4C0);
        chrObjPool[arg1].collMask = MapCollGetObjFlags();
        chrObjPool[arg1].wallHitMax = 0xFF;
        ChrTrailReset((s32)chrObjPool[arg1].trail);
        if (arg2 != 0) {
            chrObjPool[arg1].flags = 0xC8F;
        } else {
            chrObjPool[arg1].flags = 0xC8E;
        }
        chrObjPool[arg1].actFlags = 4;
        chrObjPool[arg1].blinkTimer = 0;
        chrObjPool[arg1].motSpeed = 1.0f;
        chrObjPool[arg1].unk1E = 60;
        FldHpGaugeInit(&chrObjPool[arg1].hpGauge, sysSaveData.chars[arg1].hp, sysSaveData.chars[arg1].maxHp);
        ChrModelShowExtraParts(chrPartyModels[arg1], 0);
        if (FldMapIsNoKamae() == 1) {
            ChrExtraPartsModeSet(&chrObjPool[arg1], 0);
        }
        n = ChrModelFindPart(chrPartyModels[arg1], 0x280);
        if (n != 0) {
            n->flags = 0;
        }
    }
}
