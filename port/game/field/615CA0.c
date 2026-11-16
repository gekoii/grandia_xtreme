/* Port-only C for AmapLoad (0x00615CA0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"
#include "gph.h"
#include "grxsys.h"
#include "party.h"
#include "save.h"
#include "field/state.h"
#include "field/n13.h"
#include "field/n21.h"
#include "field/n22.h"

extern struct FldMapSet *mapSetFileCopy;

void MemSet(void *, s32, s32);
void *FldMemAlloc(s32);
void MemMove128(void *, void *, s32);
void VuMtxRotate(f32 *, f32 *);
void FldSpotSetup(void *);
void ChrTenantAreaLoad(void *, u32);
void ChrTenantTrackLoad(void *, u32);
void FldEncGroupSetup(u32, void *, u32);
void EvtAreaLoad(void *, u32);
void OccsLoad(void *, u32);
void MapWallBuildChains(void *, void *, u32);
void FldTreasurePlace(void *, u32);
void FldCamConfigLoad(void *);
void MapMiniBuildFromAmap(AmapWork *);
void AmapLoad(u8 *mapData);

/* Two counts AmapWork leaves in padE2 */
#define AMAP_TRACK_COUNT (*(u16 *)((u8 *)&amapWork + 0xE2))  /* tenant tracks (unk144) */
#define AMAP_AREA_COUNT  (*(u16 *)((u8 *)&amapWork + 0xE4))  /* tenant areas (unk148) */

/* AmapCellInst words the asm sets (inside its pad3) */
#define INST_WORD(inst, off) (*(s32 *)((u8 *)(inst) + (off)))

/* Offset of a block in the map data, -1 none */
#define MDT_OFS(off) (*(s32 *)(mapData + (off)))

/* Loads the auto-map floor that AmapGenerate built: generates the layout,
   places a cell model instance per floor cell (500 units a cell), then
   loads tenants, monsters, objects, entrances, lights, event areas, walls,
   sound emitters and treasures from amapWork, and the camera, light and
   script blocks from the map data. Does nothing once amapState->loaded. */
void AmapLoad(u8 *mapData) {
    FMATRIX ident;
    FVECTOR pos;
    s32 rot[4];
    AmapCellInst *inst;
    AmapObjRec *objs;
    GphModel *mdl;
    u8 *blob;
    s32 count = 0;
    s32 cells;
    s32 type;
    s32 i;
    s32 j;
    u32 k;

    if (amapState->loaded == 1) {
        return;
    }
    EvtMapStateReset();
    cells = 0;
    /* vmr32 of vf0: the identity */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            ident[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            amapState->pad50[i * 32 + j] = 0;
        }
    }
    CamScrSetTable(0);
    FldPointLightLoad(NULL, 0);
    MapEntranceAlloc(0);
    ChrTenantAreaAlloc(0);
    ChrTenantTrackPoolAlloc(0);
    if (MDT_OFS(0x15C) != -1) {
        MapSetFileSet((struct FldMapSet *)(mapData + MDT_OFS(0x15C)));
        MapSetFileGet();
    }
    if (MDT_OFS(0x11C) != -1) {
        MapEnemyTableLoad(mapData + MDT_OFS(0x11C));
    }
    MemSet(amapWork.tiles, 0, AmapGenGetBufSize(&amapWork));
    MemSet(amapWork.cells, 0, AmapGenGetCellBufSize(&amapWork));
    amapWork.mapSet = mapSetFileCopy;
    *(PtyEnemyParams **)&amapWork.pad4[0] = ptyBattleParams;
    *(FldCharDef **)&amapWork.pad4[4] = fldCharDefTable;
    amapWork.mapNo = sysSaveData.mapNo;
    AmapGenBuild(&amapWork);

    /* one instance per cell with a model (type < 32) */
    for (i = 0; i < amapWork.height; i++) {
        for (j = 0; j < amapWork.width; j++) {
            if (!(31 < AmapGetCellType(&amapWork, i, j))) {
                cells++;
            }
        }
    }
    amapCellInsts = FldMemAlloc(cells * 0x140);
    amapState->cellInstCount = cells;
    if (amapWork.spotCount != 0) {
        if (MDT_OFS(0x16C) != -1) {
            FldSpotLoadModels((struct FldSpotHdr *)(mapData + MDT_OFS(0x16C)));
        }
        if (MDT_OFS(0x158) != -1) {
            /* the spot block's header gets the offset of amapWork's spot table */
            blob = mapData + MDT_OFS(0x158);
            *(s32 *)(blob + *(s32 *)(blob + 4) + 0xC) = (u8 *)amapWork.spots - blob;
            FldSpotSetup(blob);
        }
    }
    rot[2] = 0;
    rot[0] = 0;
    rot[3] = 0; /* not set by the asm */
    for (i = 0; i < amapWork.height; i++) {
        for (j = 0; j < amapWork.width; j++) {
            type = AmapGetCellType(&amapWork, i, j);
            if (!(type < 32)) {
                continue;
            }
            pos[3] = 1.0f;
            pos[1] = 0.0f;
            pos[0] = (f32)(j * 500);
            pos[2] = -(f32)(i * 500);
            rot[1] = (s32)((u32)-AmapGetCellRot(&amapWork, i, j) << 14);
            inst = &amapCellInsts[count];
            /* the identity with vf0 + pos (xyz) as the last row */
            __builtin_memcpy(inst->mtx, ident, 0x30);
            ((f32 *)inst->mtx)[12] = ident[3][0] + pos[0];
            ((f32 *)inst->mtx)[13] = ident[3][1] + pos[1];
            ((f32 *)inst->mtx)[14] = ident[3][2] + pos[2];
            ((f32 *)inst->mtx)[15] = ident[3][3];
            VuMtxRotate((f32 *)amapCellInsts[count].mtx, (f32 *)rot);
            mdl = amapCellModels[type];
            inst = &amapCellInsts[count];
            INST_WORD(inst, 0x100) = type;
            /* the model's first GA object (8 qwords) heads the instance */
            MemMove128(inst, mdl->parts->gaObjs, 8);
            if (amapWork.typeFlag == 0) {
                amapCellInsts[count].pad0[0] = 0;
            }
            inst = &amapCellInsts[count];
            count++;
            /* ... pointed at this instance's view matrix, the lighting
               block (scratchpad 0x70002704) and amapState +0x10 */
            INST_WORD(inst, 0x5C) = (s32)inst->viewMtx;
            INST_WORD(inst, 0x3C) = *(s32 *)0x70002704;
            INST_WORD(inst, 0x38) = (s32)amapState->unk10;
            INST_WORD(inst, 0x58) = (s32)inst->viewMtx;
        }
    }

    ChrTenantAreaAlloc(AMAP_AREA_COUNT);
    ChrTenantAreaLoad(amapWork.unk148, AMAP_AREA_COUNT);
    ChrTenantTrackPoolAlloc(AMAP_TRACK_COUNT);
    ChrTenantTrackLoad(amapWork.unk144, AMAP_TRACK_COUNT);
    ChrTenantAlloc(amapWork.monsterCount + FldMonsterGetCount());
    FldEncGroupSetup(amapWork.monsterCount + FldMonsterGetCount(), amapWork.unk14C, amapWork.monsterCount);
    ChrTenantSetupAll(amapWork.unk14C, amapWork.monsterCount);
    for (i = 0; i < amapWork.monsterCount; i++) {
        FldMonsterSetTracks(ChrTenantAt(i), 1);
    }
    FldSpotSpawnMonsters();
    FldMonsterSpawnAll();

    /* the floor's objects from the object pack */
    if (amapState->objPack != NULL) {
        amapState->objModelCount = amapWork.objCount;
        objs = amapWork.objs;
        if (amapWork.objCount >= 0x100) {
            amapState->objModelCount = 0x100;
        }
        if (objs != NULL) {
            for (i = 0; i < amapWork.objCount; i++) {
                type = FilePacGetModel((struct FilePacModelPack *)amapState->objPack,
                                       AmapObjRecGetType((u8 *)&objs[i]));
                if (type == 0) {
                    continue;
                }
                mdl = ChrModelCreate((struct ChrModelData *)type);
                amapObjModels[i] = mdl;
                mdl->flags |= 0x80;
                GphModelCalc(mdl);
                mdl = amapObjModels[i];
                mdl->flags = 0xFF7F;
                EffSpawnModelEffects(mdl);
                ChrModelSetPartPrio(amapObjModels[i], 0);
                mdl = amapObjModels[i];
                k = fldModelTable.count;
                mdl->flags = 0;
                fldModelTable.items[k] = mdl;  /* fldObjList */
                mdl->parts->flags = 0;
                fldModelTable.count = k + 1;
            }
            /* (the asm also fills a dead (255, 255, 255, 128) on its stack) */
            for (k = 0; k < amapState->objModelCount; k++) {
                mdl = amapObjModels[k];
                if (mdl == NULL) {
                    continue;
                }
                mdl->pos[0] = (f32)objs[k].x;
                mdl->pos[1] = (f32)objs[k].y;
                mdl->rot[0] = 0;
                mdl->pos[2] = -(f32)objs[k].z;
                type = AmapObjRecGetRot((u8 *)&objs[k]);
                mdl->rot[2] = 0;
                mdl->rot[1] = (s32)((u32)-type << 14);
            }
        }
    }

    MapEntranceAlloc((u16)amapWork.entranceCount);
    MapEntranceLoad(amapWork.entrances, (u16)amapWork.entranceCount);
    FldPointLightLoad(amapWork.lights, amapWork.lightCount);
    EvtAreaAlloc(amapWork.areaCount);
    OccAlloc(amapWork.occCount);
    OccsAlloc(amapWork.occsCount);
    EvtAreaLoad(amapWork.areas, amapWork.areaCount);
    OccLoad(amapWork.occ, amapWork.occCount);
    OccsLoad(amapWork.occs, amapWork.occsCount);
    MapWallSelectLayer(0);
    MapWallBuildChains(amapWork.pathRanges, amapWork.pathPoints, amapWork.pathCount);
    MapWallSelectLayer(1);
    MapWallBuildChains(amapWork.unk158, amapWork.unk15C, (u16)amapWork.unkF8);
    FldSndEmitterLoad(amapWork.sndEmitters, amapWork.sndEmitterCount);
    FldTreasurePlace(amapWork.treasures, amapWork.treasureCount);
    GsTexLoadPacket((struct GsTexPacket *)((u8 *)amapState + 0x480), sysGrxsysPac->tex08,
                    0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    if (MDT_OFS(0xC4) != -1) {
        FldCamConfigLoad(mapData + MDT_OFS(0xC4));
    }
    if (MDT_OFS(0x12C) != -1) {
        FldLightPresetLoad((s32)(mapData + MDT_OFS(0x12C)));
    }
    if (MDT_OFS(0x7C) != -1) {
        EvtScnrSetScriptTable((s32 *)(mapData + MDT_OFS(0x7C)));
    } else {
        EvtScnrSetScriptTable(NULL);
    }
    MapMiniBuildFromAmap(&amapWork);
    MapMiniAutoExploredClear();
    amapState->loaded = 1;
}
