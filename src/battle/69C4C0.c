#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/misc.h"

typedef struct {
    u8 pad0[0x4];
    s32 unk4;
} Res;

typedef struct {
    u8 pad0[0x20];
} Buf;

extern Res *btlCmdMenuRingModelData;
extern Res *btlCmdMenuLineAreaData;
extern Res *btlCmdMenuCircleAreaData;
extern Res *btlCmdMenuAllAreaAsset;
extern Res *btlCmdMenuPointData;
extern Res *btlCmdMenuAllyCursorData;
extern Res *btlCmdMenuEnemyCursorData;


void BtlCmdMenuLoad(void) {
    Buf buf;
    s32 c0 = 0xD;
    s32 c1 = 0xE;
    s32 c2 = 0xF;
    s32 c3 = 0x10;
    GphPart *p;
    GphPart *p2;
    s32 h;
    s32 i;
    s32 j;

    btlCmdMenuRingModelData = FilePacGetModel(btlSysModelPack, 0xC);
    btlCmdMenuRingInst = BtlModelCreateWithPool(btlCmdMenuRingModelData, &btlCmdMenuRingModelBuf);
    h = btlCmdMenuRingModelData->unk4;
    btlCmdMenuRingDrawList = MemAlloc32(GphDrawListSize(h));
    GphDrawListInit(h, btlCmdMenuRingDrawList);
    GphDrawListSet(btlCmdMenuRingDrawList);
    GphDrawListClear(0);
    for (i = 1; i < 9; i++) {
        p = BtlChrFindPart(btlCmdMenuRingInst, i);
        p->flags &= ~4;
    }
    p = BtlChrFindPart(btlCmdMenuRingInst, 9);
    p->flags &= ~2;
    p = BtlChrFindPart(btlCmdMenuRingInst, 10);
    p->flags &= ~7;
    btlCmdMenuRingInst->pos[0] = -100.0f;
    btlCmdMenuRingInst->pos[1] = 130.0f;
    btlCmdMenuRingInst->pos[2] = -320.0f;
    btlCmdMenuLineAreaData = FilePacGetModel(btlSysModelPack, c0);
    btlCmdMenuCircleAreaData = FilePacGetModel(btlSysModelPack, c1);
    btlCmdMenuAllAreaAsset = FilePacGetModel(btlSysModelPack, c2);
    btlCmdMenuPointData = FilePacGetModel(btlSysModelPack, c3);
    btlCmdMenuAllyCursorData = FilePacGetModel(btlSysModelPack, 0x11);
    btlCmdMenuEnemyCursorData = FilePacGetModel(btlSysModelPack, 0x12);
    BtlModelPoolReserve(btlCmdMenuLineAreaData, 1);
    BtlModelPoolReserve(btlCmdMenuCircleAreaData, 1);
    BtlModelPoolReserve(btlCmdMenuAllAreaAsset, 1);
    BtlModelPoolReserve(btlCmdMenuPointData, 6);
    BtlModelPoolReserve(btlCmdMenuAllyCursorData, 1);
    BtlModelPoolReserve(btlCmdMenuEnemyCursorData, 1);
    btlCmdMenuModelPool = BtlModelPoolAlloc(&buf);
    btlCmdMenuLineArea = BtlModelCreate(btlCmdMenuLineAreaData, 0);
    btlCmdMenuCircleArea = BtlModelCreate(btlCmdMenuCircleAreaData, 0);
    btlCmdMenuAllArea = BtlModelCreate(btlCmdMenuAllAreaAsset, 0);
    for (j = 0; j < 6; j++) {
        btlCmdMenuPoints[j] = BtlModelCreate(btlCmdMenuPointData, 0);
    }
    btlCmdMenuAllyCursor = BtlModelCreate(btlCmdMenuAllyCursorData, 0);
    btlCmdMenuEnemyCursor = BtlModelCreate(btlCmdMenuEnemyCursorData, 0);
    ChrModelPoolSet(0);
    h = btlCmdMenuLineAreaData->unk4;
    h += btlCmdMenuEnemyCursorData->unk4;
    h += btlCmdMenuPointData->unk4 * 6 + btlCmdMenuAllAreaAsset->unk4;
    h += btlCmdMenuCircleAreaData->unk4 + btlCmdMenuAllyCursorData->unk4;
    btlCmdMenuDrawList = MemAlloc32(GphDrawListSize(h));
    GphDrawListInit(h, btlCmdMenuDrawList);
    GphDrawListSet(btlCmdMenuDrawList);
    GphDrawListClear(0);
    p2 = BtlChrFindPart(btlCmdMenuLineArea, 1);
    p2->flags &= ~3;
    p2 = BtlChrFindPart(btlCmdMenuLineArea, 2);
    p2->flags &= ~3;
    p2 = BtlChrFindPart(btlCmdMenuLineArea, 3);
    p2->flags &= ~3;
    p2 = BtlChrFindPart(btlCmdMenuLineArea, 4);
    p2->flags &= ~3;
    p2 = BtlChrFindPart(btlCmdMenuCircleArea, 1);
    p2->flags &= ~4;
    p2 = BtlChrFindPart(btlCmdMenuCircleArea, 2);
    p2->flags &= ~4;
}
