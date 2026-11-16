#include "common.h"
#include "protos/viewer.h"
#include "sys.h"
#include "viewer/n35.h"

void VuDirFromAngles(f32 *, VuVec4 *);
void GphShadowCalcMtx(void *, f32 *);
void MemSet(void *, s32, s32);

void VwrInit(void) {
    f32 vec[4];
    VuVec4 rot;
    VuVec4 col;
    GphLight *l;
    s32 buf;

    MemHeapSelect(sysState.dataHeap);
    MemSet(&vwrModelList, 0, 0x404);
    MemSet(&vwrCamera, 0, 0x600);
    MemSet(vwrModelPool, 0, 0x14);
    MemSet(&vwrWork, 0, 0x4C);
    vwrWork.mainListSize = 0x1000;
    vwrWork.mainList = MemAlloc32(GphDrawListSize(0x1000));
    GphDrawListInit(vwrWork.mainListSize, vwrWork.mainList);
    GphDrawListSet(vwrWork.mainList);
    GphDrawListClear(0);
    vwrWork.shadowListSize = 0x1000;
    vwrWork.shadowList = MemAlloc32(GphDrawListSize(0x1000));
    GphDrawListInit(vwrWork.shadowListSize, vwrWork.shadowList);
    GphDrawListSet(vwrWork.shadowList);
    GphDrawListClear(0);
    vwrWork.lightCount = 4;
    vwrWork.lightList = MemAlloc16C(GphLightListSize(4));
    vwrWork.unk48 = 0;
    GphLightListInit(vwrWork.lightCount, vwrWork.lightList);
    GphLightListSet(vwrWork.lightList);
    buf = MemAlloc128(0x100000);
    MemSet((void *)buf, 0, 0x100000);
    ChrModelPoolInit(vwrModelPool, buf, 0x100000);
    ChrModelPoolSet(vwrModelPool);

    l = GphLightGet(0);
    GphLightInit(l);
    l->enable = 1;
    l->color[0] = 40.0f;
    l->color[1] = 40.0f;
    l->color[2] = 40.0f;
    l->color[3] = 128.0f;

    rot.x = 45.0f;
    rot.y = 45.0f;
    l = GphLightGet(1);
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
    GphShadowCalcMtx(vwrShadowMtx, vec);
    l->intensity = 1.0f;
    l->range = 1000.0f;

    rot.x = -45.0f;
    rot.y = -45.0f;
    l = GphLightGet(2);
    GphLightInit(l);
    l->enable = 1;
    l->color[0] = 0.0f;
    l->color[1] = 0.0f;
    l->color[2] = 64.0f;
    l->color[3] = 0.0f;
    VuDirFromAngles(vec, &rot);
    l->dir[0] = vec[0];
    l->dir[1] = vec[1];
    l->dir[2] = vec[2];
    l->dir[3] = 0.0f;
    l->intensity = 1.0f;
    l->range = 1000.0f;

    l = GphLightGet(3);
    GphLightInit(l);
    l->type = 1;
    l->enable = 0;
    l->color[1] = 0.0f;
    l->color[2] = 0.0f;
    l->color[3] = 0.0f;
    l->dir[0] = 0.0f;
    l->color[0] = 255.0f;
    l->dir[1] = 500.0f;
    l->dir[2] = -400.0f;
    l->dir[3] = 1.0f;
    l->intensity = 1.0f;
    l->range = 1000.0f;

    CamInit(&vwrCamera, 0);
    CamDebugReset(&vwrCamera);
    sysState.dbgDigit = 1;
    vwrCamera.dist = 800.0f;
    vwrCamera.lookAt[1] = 100.0f;
    sysState.dbgMenuFlags = 0;
    sysState.pauseMode = 0;
    VwrSceneSetup();
    col.x = -255.0f;
    col.y = -255.0f;
    col.z = -255.0f;
    col.w = 48.0f;
    GphShadowSetColor(&col);
}
