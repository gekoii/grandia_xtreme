#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "save.h"
#include "sys.h"
#include "party.h"
#include "field/state.h"
#include "field/n22.h"
#include "field/n17.h"

typedef struct {
    u8 pad[0x2C];
} PartyDef;

typedef struct {
    s32 pad0[2];
    s32 e[1];
} Tbl400;

void MemSet(void *, s32, s32);
void MemMove128(void *, void *, u32);

extern s32 mapModelPack;
extern Tbl400 *mapSkyInfo;
extern PartyDef fldCharModelDefs[];
extern u8 fldBtlStartTypeNormal;
extern u8 fldBtlStartTypeAmbush;
extern u8 btlPartyGbx0[];
extern u8 btlPartyGbx1[];
extern u8 btlPartyGbx2[];
extern u8 btlPartyGbx3[];
extern u8 btlEnemyGbx0[];
extern u8 btlEnemyGbx1[];
extern u8 btlEnemyGbx2[];
extern u8 btlEnemyGbx3[];

void FldBattleStart(void) {
    void *dst[4];
    s16 order16[4];
    union {
        s8 order[8];
        u16 party[4];
    } u;
    FldMapSet *ev;
    ChrObj *map;
    FldEncGroup *area;
    s32 i;
    s32 j;
    s32 k;
    s32 r2;
    s32 m;
    s32 id;
    s32 r;
    s32 bonus;

    MemSet(&btlHandoff, 0, 0x140);
    ev = MapSetFileGet();
    if (ev != NULL) {
        ptyEnemySkillCount = ev->enemySkillCount;
    } else {
        ptyEnemySkillCount = 0;
    }
    btlHandoff.partyCount = ChrPartyCountForBattle();
    btlHandoff.unk71 = 1;
    btlHandoff.stagePack = mapModelPack;
    btlHandoff.unk72 = 0;
    btlHandoff.unk128 = 0;
    btlHandoff.unk12C = 0;
    btlHandoff.ipStopOpt = sysSaveData.optIpStop;
    if (SysDemoGetMode() != 0) {
        for (i = 0; i < 8; i++) {
            u.order[i] = 0;
        }
        u.order[0] = 1;
        id = 2;
        do {
            r = VuRandInt(8);
            if (u.order[r] == 0) {
                u.order[r] = id;
                id++;
            }
        } while (id != 9);
        order16[0] = u.order[0];
        order16[1] = u.order[1];
        order16[2] = u.order[2];
        order16[3] = u.order[3];
        PtyPartySet(order16);
        PtyPartyGet(order16);
        for (i = 0; i < 8; i++) {
            ptyCharData[i].tactics[0] = 7;
            ptyCharData[i].tactics[1] = 7;
        }
        ptyPartyData->autoTactics = 1;
    }
    for (i = 0; i < 4; i++) {
        u.party[i] = sysSaveData.party[i];
    }
    PtyPartySet((s16 *)u.party);
    PtyPartyGet((s16 *)u.party);
    for (i = 0; i < 4; i++) {
        sysSaveData.party[i] = u.party[i];
    }
    dst[0] = btlPartyGbx0;
    dst[1] = btlPartyGbx1;
    dst[2] = btlPartyGbx2;
    dst[3] = btlPartyGbx3;
    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] != 0) {
            btlHandoff.partyModels[i] = &fldCharModelDefs[sysSaveData.party[i]];
            btlHandoff.unitIds[i] = sysSaveData.party[i] + 0x1000;
            btlHandoff.partyKind[i] = btlHandoff.unitIds[i];
            btlHandoff.partyLoaded[i] = 1;
            btlHandoff.unitFlags[i] = 1;
            id = (sysSaveData.party[i] - 1) * 4 + i;
            MemMove128(dst[i], (void *)sysGbxPac->ofs[id], (sysGbxPac->ofs[id + 1] - sysGbxPac->ofs[id]) >> 4);
            PtyUnitCalcParams(&ptyPartyUnits[i], 0, 0);
        }
    }
    btlHandoff.unk50 = fldState.unk2D - 1;
    k = 0;
    area = &fldEncGroups.groups[btlHandoff.unk50];
    map = ChrTenantAt(btlHandoff.unk50);
    btlHandoff.mapNo = FldBattleGetStageNo(map->sense->unk8);
    btlHandoff.unkB4 = mapSkyInfo->e[map->sense->unk8];
    if (area->slots[0].count == 0) {
        area->slots[0].count = 1;
    }
    if (area->slots[0].level == 0) {
        area->slots[0].level = 1;
    }
    dst[0] = btlEnemyGbx0;
    dst[1] = btlEnemyGbx1;
    dst[2] = btlEnemyGbx2;
    dst[3] = btlEnemyGbx3;
    for (i = 0; i < area->count; i++) {
        if (area->slots[i].count != 0) {
            id = area->slots[i].defNo;
            btlHandoff.enemyModels[i] = &fldChrPackTable[id].bind;
            btlHandoff.enemyKind[i] = fldCharDefTable[id].kind & 0xFFF;
            btlHandoff.enemyLoaded[i] = 0;
            r2 = FldBattleFindEnemyGbx(btlHandoff.enemyKind[i]);
            if (r2 >= 0) {
                m = r2 + i;
                MemMove128(dst[i], (void *)sysGbxPac->ofs[m], (sysGbxPac->ofs[m + 1] - sysGbxPac->ofs[m]) >> 4);
                btlHandoff.enemyLoaded[i] = 1;
            }
            for (j = 0; j < area->slots[i].count; j++) {
                btlHandoff.unitIds[k + 4] = btlHandoff.enemyKind[i];
                PtyUnitInitEnemy(&ptyPartyUnits[k + 4], &ptyBattleParams[k], (s16)btlHandoff.enemyKind[i], area->slots[i].level, (PtyEnemyData *)fldChrPackTable[id].pack->enemyData);
                PtyUnitCalcParams(&ptyPartyUnits[j + 4], (PtyEnemyData *)fldChrPackTable[id].pack->enemyData, &ptyBattleParams[j]);
                ptyPartyUnits[j + 4].hp = ptyPartyUnits[j + 4].maxHp;
                ptyPartyUnits[j + 4].mp = ptyPartyUnits[j + 4].maxMp;
                ptyPartyUnits[j + 4].sp = ptyPartyUnits[j + 4].maxSp;
                btlHandoff.unitFlags[k + 4] = 1;
                if ((btlHandoff.enemyKind[0] == 0x23 || btlHandoff.enemyKind[0] == 0x123) &&
                    (btlHandoff.enemyKind[1] == 0x24 || btlHandoff.enemyKind[1] == 0x124) &&
                    (btlHandoff.enemyKind[2] == 0x21 || btlHandoff.enemyKind[2] == 0x121) && i == 2 && j == i) {
                    btlHandoff.unitFlags[k + 4] = i;
                }
                if ((btlHandoff.enemyKind[0] == 0x23 || btlHandoff.enemyKind[0] == 0x123) &&
                    (btlHandoff.enemyKind[1] == 0x24 || btlHandoff.enemyKind[1] == 0x124) &&
                    (btlHandoff.enemyKind[2] == 0x21 || btlHandoff.enemyKind[2] == 0x121) &&
                    (btlHandoff.enemyKind[3] == 0x22 || btlHandoff.enemyKind[3] == 0x122) && i == 3) {
                    btlHandoff.unitFlags[k + 4] = 2;
                }
                k = (u16)(k + 1);
            }
        }
    }
    FldBattleRollInitiative();
    if ((MapGetNo() & 0xFF00) == 0xD800) {
        r = VuRandInt(100);
        if (r < 20) {
            bonus = 0;
        } else if (r < 60) {
            bonus = 8;
        } else {
            bonus = 16;
        }
    } else {
        bonus = 0;
    }
    switch (btlHandoff.encType) {
    case 0:
        btlHandoff.unk60 = 0;
        btlHandoff.unk54 = 0;
        btlHandoff.startType = fldBtlStartTypeNormal + bonus;
        break;
    case 1:
        btlHandoff.unk60 = 0;
        btlHandoff.unk54 = 0;
        btlHandoff.startType = fldBtlStartTypeNormal + bonus;
        break;
    case 2:
        btlHandoff.unk60 = 0;
        btlHandoff.unk54 = 0;
        btlHandoff.startType = fldBtlStartTypeAmbush + bonus;
        break;
    }
    btlHandoff.unk66 = bonus + 4;
    btlHandoff.unk62 = -1;
    btlHandoff.unk6A = bonus + 6;
    btlHandoff.unk64 = 0;
    btlHandoff.unk68 = 0x80;
    btlHandoff.unk6C = 0x80;
    FldBattleSetSpecialParams();
    FldSndApplyMapEnv(sysSaveData.mapNo, 1);
    if (SysDemoGetMode() != 0) {
        btlHandoff.debugStart = 1;
        btlHandoff.unk54 = 0x63;
        btlHandoff.encType = 1;
    }
}
