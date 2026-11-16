#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n12.h"
#include "field/n13.h"
#include "field/n15.h"

typedef struct {
    s32 n;
    s32 buf;
} Pool;

void VuDirFromAngles(f32 *, VuVec4 *);
void MemSet(void *, s32, s32);
void *FldMemAlloc(s32);

extern u8 fldInstancePool[];
extern Pool fldDrawListSky;
extern Pool fldDrawListParty;
extern Pool fldDrawListTenant;
extern Pool fldDrawListPartyLight;
extern Pool fldDrawListPtcl;
extern Pool fldDrawListMiniMap;
extern s32 chrObjPool;
extern u8 fldReservedTexImages[];

void FldWorkAlloc(void) {
    f32 vec[4];
    VuVec4 rot;
    GphLight *l;
    void *buf;
    s32 i;

    MemSet(&fldModelTable, 0, 0x404);
    MemSet(fldInstancePool, 0, 0x14);
    SysTexturesReset();
    buf = FldMemAlloc(0x180000);
    MemSet(buf, 0, 0x180000);
    ChrModelPoolInit(fldInstancePool, buf, 0x180000);
    ChrModelPoolSet(fldInstancePool);
    fldPacketList = FldMemAlloc(0x2004);
    MemSet(fldPacketList, 0, 0x2004);
    GphPrimListSelect(fldPacketList);

    fldMapDrawListSize[0] = 0x400;
    fldMapDrawListSize[1] = (s32)FldMemAlloc(GphDrawListSize(0x400));
    GphDrawListInit(fldMapDrawListSize[0], fldMapDrawListSize[1]);
    GphDrawListSet(fldMapDrawListSize[1]);
    GphDrawListClear(0);
    fldDrawListSky.n = 0x20;
    fldDrawListSky.buf = (s32)FldMemAlloc(GphDrawListSize(0x20));
    GphDrawListInit(fldDrawListSky.n, fldDrawListSky.buf);
    GphDrawListSet(fldDrawListSky.buf);
    GphDrawListClear(0);
    fldInstDrawListSize[0] = 0x100;
    fldInstDrawListSize[1] = (s32)FldMemAlloc(GphDrawListSize(0x100));
    GphDrawListInit(fldInstDrawListSize[0], fldInstDrawListSize[1]);
    GphDrawListSet(fldInstDrawListSize[1]);
    GphDrawListClear(0);
    fldDrawListParty.n = 0x100;
    fldDrawListParty.buf = (s32)FldMemAlloc(GphDrawListSize(0x100));
    GphDrawListInit(fldDrawListParty.n, fldDrawListParty.buf);
    GphDrawListSet(fldDrawListParty.buf);
    GphDrawListClear(0);
    fldDrawListTenant.n = 0x200;
    fldDrawListTenant.buf = (s32)FldMemAlloc(GphDrawListSize(0x200));
    GphDrawListInit(fldDrawListTenant.n, fldDrawListTenant.buf);
    GphDrawListSet(fldDrawListTenant.buf);
    GphDrawListClear(0);
    fldDrawListPartyLight.n = 0x200;
    fldDrawListPartyLight.buf = (s32)FldMemAlloc(GphDrawListSize(0x200));
    GphDrawListInit(fldDrawListPartyLight.n, fldDrawListPartyLight.buf);
    GphDrawListSet(fldDrawListPartyLight.buf);
    GphDrawListClear(0);
    fldDrawListPtcl.n = 0x200;
    fldDrawListPtcl.buf = (s32)FldMemAlloc(GphDrawListSize(0x200));
    GphDrawListInit(fldDrawListPtcl.n, fldDrawListPtcl.buf);
    GphDrawListSet(fldDrawListPtcl.buf);
    GphDrawListClear(0);
    fldDrawListMiniMap.n = 4;
    fldDrawListMiniMap.buf = (s32)FldMemAlloc(GphDrawListSize(4));
    GphDrawListInit(fldDrawListMiniMap.n, fldDrawListMiniMap.buf);
    GphDrawListSet(fldDrawListMiniMap.buf);
    GphDrawListClear(0);
    fldState.lightCount = 0x10;
    fldState.lightList = (s32)FldMemAlloc(GphLightListSize(0x10));
    GphLightListInit(fldState.lightCount, fldState.lightList);
    GphLightListSet(fldState.lightList);

    GphViewSetup(6, 0x20, 0xF4, 0xB4, 0xA0, 8000.0f, 1.0f, 70.0f);
    CamInit(&camField[0], 0);
    CamInit(&camField[1], 6);
    CamDebugReset(camField);

    l = (GphLight *)GphLightGet(0);
    GphLightInit(l);
    l->color[0] = 64.0f;
    l->color[1] = 64.0f;
    l->color[2] = 64.0f;
    l->enable = 1;
    l->color[3] = 128.0f;

    rot.x = 45.0f;
    rot.y = 45.0f;
    l = (GphLight *)GphLightGet(1);
    GphLightInit(l);
    l->enable = 1;
    l->color[0] = 128.0f;
    l->color[1] = 128.0f;
    l->color[2] = 128.0f;
    l->color[3] = 0.0f;
    VuDirFromAngles(vec, &rot);
    l->dir[0] = vec[0];
    l->dir[1] = vec[1];
    l->dir[2] = vec[2];
    l->dir[3] = 0.0f;
    l->intensity = 1.0f;
    l->range = 1000.0f;

    rot.x = -45.0f;
    rot.y = -45.0f;
    l = (GphLight *)GphLightGet(2);
    GphLightInit(l);
    l->color[2] = 64.0f;
    l->enable = 1;
    l->color[0] = 0.0f;
    l->color[1] = 0.0f;
    l->color[3] = 0.0f;
    VuDirFromAngles(vec, &rot);
    l->dir[0] = vec[0];
    l->dir[1] = vec[1];
    l->dir[2] = vec[2];
    l->range = 1000.0f;
    l->intensity = 1.0f;
    l->dir[3] = 0.0f;

    for (i = 0; i < 4; i++) {
        l = (GphLight *)GphLightGet(i + 3);
        GphLightInit(l);
        l->type = 1;
        l->enable = 0;
    }

    chrObjPool = ChrObjArrayAlloc(0x20);
    EvtScnrStateReset();
    FldModulesInit();
    EffNop(0x10);
    GphTexRegisterAt(fldReservedTexImages, 0xFC);
    GphTexRegisterAt(fldReservedTexImages + 0x680, 0xFD);
    GphTexRegisterAt(fldReservedTexImages + 0xD00, 0xFE);
}
