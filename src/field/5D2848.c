#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "save.h"
#include "sys.h"
#include "field/state.h"
#include "field/n15.h"
#include "field/n17.h"

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x20];
    s32 unk28;
    u8 pad2C[0x3C];
    s32 unk68;
} Sub108;

extern void MemSet(void *, s32, s32);
extern u8 *FldMemAlloc(s32);
extern void memcpy(void *, u32, s32);
extern void PtyEnemyDataRelocate(s32);
extern void AmapLoad(u8 *);
extern void MapDataLoad(u8 *);
extern void MapWallCollectInArea(s32);

extern s32 mapLoadWaitFrames;
extern s32 mapLoadStartTime;
extern s32 mapLoadTime;
extern f32 fldPerfTimes[];
extern u8 *mapData;
extern s32 mapModelPack;
extern Sub108 *mapSkyInfo;

void MapLoadUpdate(void) {
    char buf[0x20];
    MapLoadState *st;
    s32 size;
    s32 r;
    u32 i;
    s32 j;
    u32 off;
    MapOffsetList *list;
    ChrPack *obj;
    Sub108 *sub;
    s32 id;

    MemHeapSelect(sysState.dataHeap);
    switch (mapLoadState.state) {
    case 0:
        if ((r = FileGetReadState()) == -1 && mapLoadState.sub == r) {
            btlHandoff.softResetLock = 1;
            mapLoadStartTime = GsGetVblankCount();
            SndSeStopAll();
            SndSeSuspend();
            MapEntranceSetKeepYaw(camField[0].yaw);
            if (mapLoadState.largeId != 0) {
                if (mapLoadState.largeBuf < FldMemGetCur()) {
                    mapLoadState.largeId = 0;
                    mapLoadState.largeBuf = mapLoadState.largeSize = -1;
                }
                if (mapLoadState.largeId != 0 && mapLoadState.largeId == mapNo.file) {
                    mapLoadState.which = 0;
                    mapLoadState.state++;
                    break;
                }
            }
            if (mapLoadState.smallId != 0 && mapLoadState.smallId == mapNo.file) {
                mapLoadState.which = 1;
                mapLoadState.state++;
                break;
            }
            MemSet(buf, 0, 0x20);
            MapMakeMdtPath(mapNo.file, buf);
            size = FileGetSizeAligned(buf);
            if (size <= 0x11FEFF) {
                mapLoadState.which = 1;
                mapLoadState.smallId = mapNo.file;
                mapLoadState.smallSize = size;
                mapLoadState.smallBuf = FldMemGetSubBuf(size);
                FileReadStart(buf, mapLoadState.smallBuf);
            } else {
                mapLoadState.which = 0;
                mapLoadState.largeId = mapNo.file;
                mapLoadState.largeSize = size;
                mapLoadState.largeBuf = FldMemGetTailBuf(size);
                FileReadStart(buf, mapLoadState.largeBuf);
            }
            if (mapLoadState.which == 0 && FldMemTailOverlaps(mapLoadState.largeSize) != 0) {
                FldMemReset();
                FldModulesInit();
            }
            mapLoadWaitFrames = 0;
            mapLoadState.state++;
        }
        break;
    case 1:
        mapLoadWaitFrames++;
        mapLoadTime = GsGetVblankCount();
        if (GphFadeIsActive(fldFades) == 1) {
            break;
        }
        r = FileGetReadState();
        if (fldState.mode == 1) {
            break;
        }
        if (r != -1) {
            break;
        }
        if (SysDemoGetMode() == 0xD) {
            TxtFontSetImageEnable(1);
            SysDemoSetReady();
            SysDemoSetMode(0);
        }
        if (SysDemoGetMode() >= 0x1B) {
            SysDemoSetReady();
        }
        fldState.mode = 0;
        fldPerfTimes[0] = GsGetFrameTimePercent();
        if ((mapNo.cur & 0xFF00) != (mapNo.prev & 0xFF00)) {
            EvtAreaStateReset();
        }
        fldPerfTimes[1] = GsGetFrameTimePercent();
        EvtMapStateReset();
        EvtSetMapNo(mapNo.cur, mapNo.prev);
        fldPerfTimes[2] = GsGetFrameTimePercent();
        SysMfifoStop(0);
        SysMfifoStart();
        fldPerfTimes[3] = GsGetFrameTimePercent();
        FldMemReset();
        fldPerfTimes[4] = GsGetFrameTimePercent();
        st = &mapLoadState;
        st->size = FileCmpGetSize(st->which == 0 ? st->largeBuf : st->smallBuf);
        fldPerfTimes[5] = GsGetFrameTimePercent();
        if (mapLoadState.size == 0) {
            if (mapLoadState.which == 0) {
                mapLoadState.size = mapLoadState.largeSize;
                mapLoadState.data = FldMemAlloc(mapLoadState.largeSize);
                memcpy(mapLoadState.data, mapLoadState.largeBuf, mapLoadState.size);
            } else {
                mapLoadState.size = mapLoadState.smallSize;
                mapLoadState.data = FldMemAlloc(mapLoadState.smallSize);
                memcpy(mapLoadState.data, mapLoadState.smallBuf, mapLoadState.size);
            }
        } else {
            mapLoadState.data = FldMemAlloc(mapLoadState.size);
            if (mapLoadState.which == 0) {
                FileDecompress(mapLoadState.largeBuf, mapLoadState.data, 1);
            } else {
                FileDecompress(mapLoadState.smallBuf, mapLoadState.data, 1);
            }
        }
        fldPerfTimes[6] = GsGetFrameTimePercent();
        FldWorkAlloc();
        fldPerfTimes[7] = GsGetFrameTimePercent();
        mapData = mapLoadState.data;
        fldPerfTimes[9] = GsGetFrameTimePercent();
        fldPerfTimes[10] = GsGetFrameTimePercent();
        off = *(s32 *)(mapData + 0xF8);
        if (off != 0xFFFFFFFF) {
            mapCharPackList = (MapOffsetList *)(mapData + off);
            for (i = 0; i < mapCharPackList->count; i++) {
                obj = FilePacCharRelocate(mapData + mapCharPackList->offs[i]);
                fldChrPackTable[i].pack = obj;
                ChrPackBind(&fldChrPackTable[i].bind, obj);
                if (fldChrPackTable[i].pack->enemyData != 0) {
                    PtyEnemyDataRelocate(fldChrPackTable[i].pack->enemyData);
                }
            }
        }
        off = *(s32 *)(mapData + 0x174);
        if (off != 0xFFFFFFFF) {
            list = (MapOffsetList *)(mapData + off);
            for (i = 0; i < list->count; i++) {
                obj = FilePacCharRelocate(mapData + list->offs[i]);
                mapExtraCharPacks[i].pack = obj;
                ChrPackBind(&mapExtraCharPacks[i].bind, obj);
                if (mapExtraCharPacks[i].pack->enemyData != 0) {
                    PtyEnemyDataRelocate(mapExtraCharPacks[i].pack->enemyData);
                }
            }
        }
        fldPerfTimes[11] = GsGetFrameTimePercent();
        if (*(u32 *)(mapData + 0xF0) != 0xFFFFFFFF) {
            mapModelPack = FilePacModelRelocate(mapData + *(u32 *)(mapData + 0xF0));
            if (mapModelPack != 0) {
                FilePacModelBindTextures(mapModelPack);
            }
            if (*(u32 *)(mapData + 0x108) != 0xFFFFFFFF) {
                mapSkyInfo = sub = (Sub108 *)(mapData + *(u32 *)(mapData + 0x108));
                if (sub->unk4 != 0) {
                    FldSkyLoad(mapModelPack, sub->unk28, sub->unk68);
                }
            }
        }
        fldPerfTimes[12] = GsGetFrameTimePercent();
        ChrPartyReload();
        fldPerfTimes[8] = GsGetFrameTimePercent();
        camCtl.target = (ChrObj *)ChrGetByNo(1);
        if (*(s32 *)(mapData + 0x120) == 1) {
            AmapLoadCellModels(mapData + *(s32 *)(mapData + 0x124));
            AmapLoadObjPack(mapData + *(s32 *)(mapData + 0x128));
            AmapLoad(mapData);
        } else {
            MapDataLoad(mapData);
        }
        fldPerfTimes[13] = GsGetFrameTimePercent();
        fldPerfTimes[14] = GsGetFrameTimePercent();
        for (j = 0; j < ChrTenantGetCount(); j++) {
            MapWallCollectInArea(ChrTenantAt(j));
        }
        fldState.unk2D = 0;
        MapEntrancePlacePlayer(mapNo.entrance);
        fldPerfTimes[15] = GsGetFrameTimePercent();
        CamResetToPlayer();
        fldPerfTimes[16] = GsGetFrameTimePercent();
        EvtAreaUpdate(padPort0);
        OccsUpdate();
        OccUpdate();
        EvtAreaClearAllMoves();
        fldPerfTimes[18] = GsGetFrameTimePercent();
        CamScrSaveCamera(0);
        CamScrSaveCamera(1);
        MapRippleLoad();
        fldPerfTimes[19] = GsGetFrameTimePercent();
        mapLoadState.state = 10;
        mapLoadState.sub = 0;
        mapMotionTable = 0;
        fldPerfTimes[20] = GsGetFrameTimePercent();
        EvtScnrCall(0x1000);
        do {
            EvtScnrPadUpdate(padPort0);
            EvtScnrMain();
            EvtScnrFrameUpdate();
        } while (EvtScnrIsRunning() != 0);
        fldPerfTimes[21] = GsGetFrameTimePercent();
        EvtScnrCall(0x1001);
        fldPerfTimes[22] = GsGetFrameTimePercent();
        sysState.frameSkip = 0;
        fldState.mode = 0;
        mapLoadState.time = fldPerfTimes[22] - fldPerfTimes[0];
        if ((mapNo.cur & 0xFF00) == 0x1000) {
            WmapStart(0);
            EvtScnrEnd();
        }
        if (SysDemoGetMode() != 0) {
            FldTitleStart();
            EvtScnrEnd();
            mapLoadState.sub = -1;
            mapLoadState.state = -1;
        } else if (sysSaveData.optVibration == 0) {
            PadSetVibEnabled(1);
        } else {
            PadSetVibEnabled(0);
        }
        btlHandoff.softResetLock = 0;
        AmapFloorBannerStart(0);
        ChrFollowersSnapToLeader();
        break;
    case 10:
        sysState.frameSkip = 1;
        mapLoadTime = GsGetVblankCount();
        mapLoadState.state++;
        break;
    case 11:
        if (fldState.pendingBattleStart == 1 || sysState.progChangeReq == 1) {
            mapLoadState.state = -1;
        } else if (EvtScnrIsRunning() == 0) {
            EvtScnrCall(0xE000);
            mapLoadState.state = -1;
        }
        break;
    }

    switch ((u32)mapLoadState.sub) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (fldState.pendingBattleStart == 1) {
            mapLoadState.sub = -1;
            SndSeResume();
        } else if (sysState.progChangeReq == 1) {
            mapLoadState.sub = -1;
        } else if (FileGetReadState() == -1 && SndSeBankLoadGetState() == -1) {
            if (mapLoadState.state != 1) {
                MemSet(buf, 0, 0x20);
                id = FldSndMakeMapSeName(mapFileNo, buf, mapLoadState.sub);
                if (id != 0) {
                    if (FldSndMapSeIsLoaded(mapLoadState.sub, id) == 0) {
                        SndSeBankLoadStart(buf, 0, id, 0);
                        FldSndSetMapSe(mapLoadState.sub, id);
                    }
                }
                mapLoadState.sub++;
            }
        }
        break;
    case 4:
    case 5:
    case 6:
        if (FileGetReadState() == -1 && SndSeBankLoadGetState() == -1) {
            if (mapLoadState.state != 1) {
                if (sysSaveData.party[mapLoadState.sub - 3] != 0) {
                    MemSet(buf, 0, 0x20);
                    id = FldSndMakePartySeName(mapLoadState.sub - 4, buf, sysSaveData.party[mapLoadState.sub - 3]);
                    if (id != FldSndGetPartySe(mapLoadState.sub - 4)) {
                        SndSeBankLoadStart(buf, 0, id, 0);
                        FldSndSetPartySe(mapLoadState.sub - 4, id);
                    }
                }
                mapLoadState.sub++;
            }
        }
        break;
    case 7:
        r = SndSeBankLoadGetState();
        if (r == -1) {
            if (sysState.progChangeReq == 0) {
                FldSndApplyMapEnv(mapFileNo, 0);
                SndSeResume();
                SndSeSetRequestEnabled(1);
                SndBgmSetEnabled(1);
            }
            mapLoadState.sub = r;
        }
        break;
    }

    MemHeapSelect(sysState.dataHeap);
    sysState.unk11 = 1;
    if (mapLoadState.sub == -1 && (mapLoadState.state >= 10 || mapLoadState.state == mapLoadState.sub)) {
        sysState.unk11 = 0;
    }
}
