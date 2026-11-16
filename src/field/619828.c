#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "sys.h"
#include "gph.h"
#include "field/state.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void FldBattleReturn(void) {
    s16 party[4];
    ChrObj *pc;
    ChrObj *ent;
    s32 i;
    u8 n;

    pc = ChrGetByNo(1);
    ent = ChrTenantGet(btlHandoff.unk50);
    PtyPartyStoreStats();
    MapMiniResetDelay();
    FldThreatGaugeReset();
    for (i = 0; i < 4; i++) {
        party[i] = 0;
        if (btlHandoff.unitIds[i] != 0) {
            party[i] = sysSaveData.party[i] = btlHandoff.unitIds[i] - 0x1000;
        }
    }
    PtyPartySet(party);
    PtyPartyGet(party);
    for (i = 0; i < 4; i++) {
        sysSaveData.party[i] = party[i];
    }
    FldHpGaugeInit(&pc->hpGauge, sysSaveData.chars[0].hp, sysSaveData.chars[0].maxHp);
    copyq(&btlHandoff.pos, &pc->model->pos);
    if (btlHandoff.result == 2) {
        if (ent != NULL && (ent->flags & 2)) {
            ent->flags = -3;
        }
        EvtMapFlagSet(btlHandoff.unk50, 1);
        if (ent->sense->flagId != 0) {
            EvtDungeonFlagSet(ent->sense->flagId, 1);
        }
    }
    if (pc != NULL) {
        if (EvtSystemFlagGet(0x65) == 0) {
            pc->blinkTimer = 0xB4;
            pc->actFlags |= 0x100;
        }
        if (btlHandoff.dropCount != 0) {
            SndSePlay(0x7D7, 0, 0, 0x1E, 0);
            EffSpawnPickup(&pc->model->pos);
        }
    }
    btlHandoff.result = 0;
    MapCacheClearSmall();
    n = fldState.unk2C;
    if (n != 0) {
        fldState.unk2C = 0;
        btlHandoff.endFade = n - 1;
    }
    /* GphScreenFxSet goes through a cast with the float declared third: EABI
     * loads arguments in declaration order, and the real (u8, s32, u32, f32)
     * order schedules the 0.0f load differently */
    switch (btlHandoff.enemyKind[0]) {
    case 0xC1:
        ((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 6, 0.0f, 1, 0);
        SysScreenCaptureRequest();
        break;
    case 0x61:
    case 0x161:
        GphFadeSetColor(fldFades, 255.0f, 255.0f, 255.0f, 128.0f);
        break;
    case 0x65:
    case 0x165:
        ((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 4, 0.0f, 1, 0x14);
        SysScreenCaptureRequest();
        GphFadeSetColor(fldFades, 0.0f, 0.0f, 0.0f, 0.0f);
        FldSndBgmPlayFade(0xFFFFFFFF);
        break;
    default:
        ((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 6, 0.0f, 0, 0);
        ((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 4, 0.0f, 1, 0x14);
        SysScreenCaptureRequest();
        FldSndBgmPlayFade(0xFFFFFFFF);
        switch (btlHandoff.endFade) {
        case 1:
            GphFadeSetColor(fldFades, 0.0f, 0.0f, 0.0f, 128.0f);
            GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, 300.0f);
            break;
        case 2:
            GphFadeSetColor(fldFades, 255.0f, 255.0f, 255.0f, 128.0f);
            GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, 300.0f);
            break;
        case 3:
            GphFadeSetColor(fldFades, 0.0f, 0.0f, 0.0f, 128.0f);
            break;
        case 4:
            GphFadeSetColor(fldFades, 255.0f, 255.0f, 255.0f, 128.0f);
            break;
        case 5:
            ((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 6, 0.0f, 1, 0);
            break;
        }
        break;
    }
    btlHandoff.endFade = 0;
    FldSndApplyMapEnv(sysSaveData.mapNo, 0);
}
