#include "common.h"
#include "protos/field.h"
#include "field/n13.h"
#include "field/n14.h"
#include "field/n15.h"

typedef struct {
    u8 pad0[0x8];
    u32 unk8;
    u32 unkC;
    u8 pad10[0x8];
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
    u32 unk38;
    u32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    u8 pad58[0x4];
    u32 unk5C;
    u32 unk60;
    u32 unk64;
    u32 unk68;
    u32 unk6C;
    u32 unk70;
    u32 unk74;
    u8 pad78[0x4];
    u32 unk7C;
    u32 unk80;
    u32 unk84;
    u8 pad88[0x4];
    u32 unk8C;
    u8 pad90[0x4];
    u32 unk94;
    u32 unk98;
    u32 unk9C;
    u8 padA0[0x4];
    u32 unkA4;
    u8 padA8[0x1C];
    u32 unkC4;
    u8 padC8[0x4];
    u32 unkCC;
    u8 padD0[0x48];
    u32 unk118;
    u32 unk11C;
    u8 pad120[0xC];
    u32 unk12C;
    u8 pad130[0x4];
    u32 unk134;
    u32 unk138;
    u32 unk13C;
    u32 unk140;
    u32 unk144;
    u8 pad148[0x4];
    u32 unk14C;
    u8 pad150[0x4];
    u32 unk154;
    u32 unk158;
    u32 unk15C;
    u32 unk160;
    u32 unk164;
    u32 unk168;
    u32 unk16C;
    u32 unk170;
    u32 unk174;
    u32 unk178;
    u32 unk17C;
    u32 unk180;
    u32 unk184;
} FieldHdr;

#define NONE 0xFFFFFFFF
#define OFS(f) ((u8 *)h + h->f) /* the header holds byte offsets from its own start */

void FldSpotSetup(void *);
void MapModelPlace(void *, u32);
void ChrTenantAreaLoad(void *, u32);
void ChrTenantTrackLoad(void *, u32);
void FldEncGroupSetup(s32, void *, u32);
void OccsLoad(void *, u32);
void EvtAreaLoad(void *, u32);
void MapWallBuildChains(void *, void *, u32);
void MapWallBuildCylinders(void *, u32);
void FldCamConfigLoad(void *);
void FldTreasurePlace(void *, u32);
void ChrMotBindFromMap(void *, void *, u32);
void PtclFldLoad(void *);


void MapDataLoad(FieldHdr *h) {
    void *obj;
    void *tbl;
    u32 n;
    s32 i;
    u8 *p;

    if (h == NULL) {
        return;
    }
    if (h->unk15C != NONE) {
        MapSetFileSet(OFS(unk15C));
    }
    if (h->unk11C != NONE) {
        MapEnemyTableLoad(OFS(unk11C));
    }
    if (h->unk178 != NONE) {
        p = OFS(unk178);
        mapBlock178 = p;
        mapBlock178Data = p + ((u32 *)p)[2];
    }
    if (h->unk184 != NONE) {
        MapMiniLoad(OFS(unk184));
    }
    if (h->unk16C != NONE) {
        FldSpotLoadModels(OFS(unk16C));
    }
    if (h->unk158 != NONE) {
        FldSpotSetup(OFS(unk158));
    }
    if (h->unk118 != NONE) {
        obj = ChrGetByNo(1);
        tbl = FilePacMotionRelocate(OFS(unk118));
        for (i = 0; i < 6; i++) {
            ChrMotTableSet(obj, FilePacGetMotion(tbl, i), i + 9);
        }
    }
    if (h->unk170 != NONE) {
        if (h->unk174 != NONE) {
            MiniLoad(OFS(unk170), 0);
        } else {
            MiniLoad(OFS(unk170), 1);
        }
    }
    n = h->unk18;
    if (n != 0) {
        MapModelPlace(OFS(unk1C), n);
    }
    if (h->unk154 != NONE) {
        MapCollModelLoad(OFS(unk154), 0);
    } else {
        MapCollModelSet(MapModelGetObj(0), 0);
    }
    if (h->unk14C != NONE) {
        MapCollModelLoad(OFS(unk14C), 1);
    } else {
        MapCollModelSet(0, 1);
    }
    n = h->unk8;
    MapEntranceAlloc(n);
    if (h->unkC != NONE) {
        MapEntranceLoad(OFS(unkC), n);
    }
    n = h->unk38;
    ChrTenantAreaAlloc(n);
    if (h->unk3C != NONE) {
        ChrTenantAreaLoad(OFS(unk3C), n);
    }
    n = h->unk40;
    ChrTenantTrackPoolAlloc(n);
    if (h->unk44 != NONE) {
        ChrTenantTrackLoad(OFS(unk44), n);
    }
    n = h->unk48;
    ChrTenantAlloc(n + FldMonsterGetCount() + MiniGetTenantCount());
    if (h->unk4C != NONE) {
        FldEncGroupSetup(n + FldMonsterGetCount(), OFS(unk4C), n);
        ChrTenantSetupAll(OFS(unk4C), n);
    }
    FldSpotSpawnMonsters();
    FldMonsterSpawnAll();
    MiniConcertCarrosCreate();
    if (h->unk74 != NONE) {
        FldPointLightLoad(OFS(unk74), h->unk70);
    } else {
        FldPointLightLoad(NULL, 0);
    }
    MapModelAttachEffects();
    n = h->unk28;
    OccAlloc(n);
    if (h->unk2C != NONE) {
        OccLoad(OFS(unk2C), n);
    } else {
        OccSetCount(0);
    }
    n = h->unk30;
    OccsAlloc(n);
    if (h->unk34 != NONE) {
        OccsLoad(OFS(unk34), n);
    }
    n = h->unk20;
    EvtAreaAlloc(n);
    if (h->unk24 != NONE) {
        EvtAreaLoad(OFS(unk24), n);
    }
    MapWallSelectLayer(0);
    n = h->unk60;
    if (h->unk64 != NONE) {
        MapWallBuildChains(OFS(unk64), OFS(unk5C), n);
    }
    n = h->unk68;
    if (h->unk6C != NONE) {
        MapWallBuildCylinders(OFS(unk6C), n);
    }
    MapWallSelectLayer(1);
    n = h->unk138;
    if (h->unk13C != NONE) {
        MapWallBuildChains(OFS(unk13C), OFS(unk134), n);
    }
    n = h->unk140;
    if (h->unk144 != NONE) {
        MapWallBuildCylinders(OFS(unk144), n);
    }
    if (h->unk7C != NONE) {
        EvtScnrSetScriptTable(OFS(unk7C));
    } else {
        EvtScnrSetScriptTable(NULL);
    }
    if (h->unkA4 != NONE) {
        CamScrSetTable(OFS(unkA4));
    } else {
        CamScrSetTable(NULL);
    }
    n = h->unk50;
    if (h->unk54 != NONE) {
        FldSndEmitterLoad(OFS(unk54), n);
    }
    if (h->unkCC != NONE) {
        FldCamConfigSetAux(OFS(unkCC));
    } else {
        FldCamConfigSetAux(NULL);
    }
    if (h->unk12C != NONE) {
        FldLightPresetLoad(OFS(unk12C));
    }
    if (h->unkC4 != NONE) {
        FldCamConfigLoad(OFS(unkC4));
    }
    if (h->unk9C != NONE) {
        FldTreasurePlace(OFS(unk9C), h->unk98);
    }
    fldPathCount = h->unk80;
    fldPathTable = (FldPath *)OFS(unk84);
    fldPathNodeData = (MapPathNode *)OFS(unk8C);
    fldPathEventData = (FldPathEvent *)OFS(unk94);
    if (h->unk164 != NONE) {
        ChrMotBindFromMap(OFS(unk164), OFS(unk168), h->unk160);
    }
    if (h->unk180 != NONE) {
        PtclFldLoad(OFS(unk180));
    }
    if (h->unk17C != NONE) {
        FldOverlayImgLoad(OFS(unk17C));
    }
}
