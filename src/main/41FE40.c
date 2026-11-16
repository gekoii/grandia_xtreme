#include "common.h"
#include "protos/main.h"
#include "grxsys.h"
#include "sys.h"
#include "party.h"
#include "chr.h"

extern char dbgPathTexEnv0[];
extern char dbgPathTexEnv1[];
extern char dbgPathTexEnv2[];
extern char dbgPathStage[];
extern ChrPackBinding dbgPartyModels[];
extern ChrPackBinding dbgEnemyModels[];
extern void *dbgStagePack;
extern u8 btlPartyGbx0[];
extern u8 btlPartyGbx1[];
extern u8 btlPartyGbx2[];
extern u8 btlPartyGbx3[];
extern u8 btlEnemyGbx0[];
extern u8 btlEnemyGbx1[];
extern u8 btlEnemyGbx2[];
extern u8 btlEnemyGbx3[];

void MemSet(void *, s32, s32);
void PtyEnemyDataRelocate(void *);
char *strcat(char *, char *);

void DbgBattleSetup(void) {
    char buf[0x40];
    s32 ids[4];
    s32 i;
    s32 q;
    s32 k;
    s32 id;
    s32 id2;
    u32 jj;
    ChrPack *res;
    u32 a;
    u32 b;

    i = 0;
    ptyEnemySkillCount = 0;
    MemHeapSelect(sysState.dataHeap);
    MemSet(buf, 0, 0x40);
    for (; i < sysGrxsysPac->sndEnv->envCount; i++) {
        if (sysGrxsysPac->sndEnv->envs[i].id == 11) {
            SndSetDistanceScale(sysGrxsysPac->sndEnv->envs[i].distScale);
            SndSetEffect(&sysGrxsysPac->sndEnv->envs[i]);
            break;
        }
    }
    MemSet(&btlHandoff, 0, 0x140);
    GphEnvMapClear();
    k = 0;
    if (FileGetSizeAligned(dbgPathTexEnv0) == -1) {
        id = MemAlloc64(-1);
        FileRead(dbgPathTexEnv0, id);
        GphTexDataRelocate(id);
        GphEnvMapSetTex(0, id);
    }
    if (FileGetSizeAligned(dbgPathTexEnv1) == -1) {
        id = MemAlloc64(-1);
        FileRead(dbgPathTexEnv1, id);
        GphTexDataRelocate(id);
        GphEnvMapSetTex(1, id);
    }
    if (FileGetSizeAligned(dbgPathTexEnv2) == -1) {
        id = MemAlloc64(-1);
        FileRead(dbgPathTexEnv2, id);
        GphTexDataRelocate(id);
        GphEnvMapSetTex(2, id);
    }
    for (i = 0; i < 4; i++) {
        if (sysIni.mapSeBank[i] != 0) {
            MemSet(buf, 0, 0x40);
            DbgSePathMake(buf, sysIni.mapSeBank[i]);
            SndSeBankLoad(buf, 0, sysIni.mapSeBank[i], 0);
        }
    }
    for (i = 0; i < 4; i++) {
        ids[i] = 0;
        if (sysIni.monsNum[i] == 0) {
            continue;
        }
        MemSet(buf, 0, 0x40);
        DbgChrPathMake(buf, sysIni.monsNo[i]);
        id2 = FileGetSizeAligned(buf);
        if (id2 == -1) {
            continue;
        }
        ids[i] = MemAlloc64(id2);
        FileRead(buf, ids[i]);
        res = FilePacCharRelocate(ids[i]);
        ChrPackBind(&dbgEnemyModels[i], res);
        if (res->enemyData != 0) {
            PtyEnemyDataRelocate((void *)res->enemyData);
        }
        btlHandoff.enemyModels[i] = &dbgEnemyModels[i];
        MemSet(buf, 0, 0x40);
        DbgEnemyGbxPathMake(buf, sysIni.monsNo[i], i);
        if (FileGetSizeAligned(buf) != 0) {
            switch (i) {
            case 0:
                FileRead(buf, (s32)btlEnemyGbx0);
                break;
            case 1:
                FileRead(buf, (s32)btlEnemyGbx1);
                break;
            case 2:
                FileRead(buf, (s32)btlEnemyGbx2);
                break;
            case 3:
                FileRead(buf, (s32)btlEnemyGbx3);
                break;
            }
            btlHandoff.enemyLoaded[i] = 1;
        } else {
            btlHandoff.enemyLoaded[i] = 0;
        }
        btlHandoff.enemyKind[i] = sysIni.monsNo[i] & 0xFFF;
        for (jj = 0; jj < sysIni.monsNum[i]; jj++) {
            btlHandoff.unitIds[k + 4] = btlHandoff.enemyKind[i];
            PtyUnitInitEnemy(&ptyPartyUnits[k + 4], &ptyBattleParams[k], (s16)btlHandoff.enemyKind[i], sysIni.monsLv[i], (PtyEnemyData *)res->enemyData);
            k++;
            PtyUnitCalcParams(&ptyPartyUnits[jj + 4], (PtyEnemyData *)res->enemyData, &ptyBattleParams[jj]);
        }
    }
    for (q = 0; q < 4; q++) {
        if (sysIni.partyExt[q] == 0) {
            continue;
        }
        MemSet(buf, 0, 0x40);
        DbgPartyGbxPathMake(buf, sysIni.partyNum[q] - 0x1001, q);
        if (FileGetSizeAligned(buf) != 0) {
            switch (q) {
            case 0:
                FileRead(buf, (s32)btlPartyGbx0);
                break;
            case 1:
                FileRead(buf, (s32)btlPartyGbx1);
                break;
            case 2:
                FileRead(buf, (s32)btlPartyGbx2);
                break;
            case 3:
                FileRead(buf, (s32)btlPartyGbx3);
                break;
            }
            btlHandoff.partyLoaded[q] = 1;
        } else {
            btlHandoff.partyLoaded[q] = 0;
        }
        MemSet(buf, 0, 0x40);
        if (q != 0 && sysIni.partyNum[q] != 0x1001) {
            SndSeBankLoad(buf, 0, DbgPartySePathMake(q, buf, sysIni.partyNum[q]), 0);
        }
        PtyCharInit(sysIni.partyNum[q], &sysIni.party[q]);
        PtyUnitInitFromChar(&ptyPartyUnits[q], sysIni.partyNum[q]);
        PtyUnitCalcParams(&ptyPartyUnits[q], 0, 0);
        ChrPackBind(&dbgPartyModels[q], sysPartyPac.packs[sysIni.partyNum[q] - 0x1001]);
    }
    for (i = 0; i < 4; i++) {
        if (sysIni.partyExt[i] != 0) {
            btlHandoff.partyModels[i] = &dbgPartyModels[i];
            btlHandoff.unitIds[i] = sysIni.partyNum[i];
            btlHandoff.unitFlags[i] = 1;
            btlHandoff.partyCount++;
        }
    }
    btlHandoff.partyKind[0] = btlHandoff.unitIds[0];
    btlHandoff.partyKind[1] = btlHandoff.unitIds[1];
    btlHandoff.partyKind[2] = btlHandoff.unitIds[2];
    btlHandoff.partyKind[3] = btlHandoff.unitIds[3];
    btlHandoff.encType = VuRandInt(2);
    if (btlHandoff.encType == 2) {
        btlHandoff.startType = 0x24;
        btlHandoff.unk60 = 0xFF;
    } else {
        a = VuRandInt(2);
        b = VuRandInt(1);
        btlHandoff.unk60 = 0x80;
        btlHandoff.startType = btlHandoff.encType * 2 + a * 12 + b * 4;
    }
    btlHandoff.unk62 = -1;
    btlHandoff.unk66 = 4;
    btlHandoff.unk6A = 6;
    btlHandoff.unk64 = 0;
    btlHandoff.unk68 = 0x80;
    btlHandoff.unk6C = 0x80;
    MemSet(buf, 0, 0x40);
    strcat(buf, dbgPathStage);
    id = MemAlloc64(FileGetSizeAligned(buf));
    FileRead(buf, id);
    btlHandoff.unk71 = 1;
    btlHandoff.mapNo = 1;
    btlHandoff.unkB4 = 0;
    dbgStagePack = FilePacModelRelocate(id);
    if (dbgStagePack != NULL) {
        FilePacModelBindTextures(dbgStagePack);
    }
    btlHandoff.stagePack = dbgStagePack;
    btlHandoff.unk72 = 0;
    MemHeapReset(sysState.battleMagicHeap);
}
