/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/viewer.h"
#include "sys.h"
#include "viewer/n35.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    VuVec4 pos;
    s32 x10;
    s32 x14;
    s32 x18;
} Tmp;

void DbgLightAngleGet(u8, VuVec4 *);
void VuDirFromAngles(f32 *, VuVec4 *);
void GphShadowCalcMtx(void *, f32 *);
void VuMtxStackReset(void);
void VuMtxStackLoad(void *);
void VuMtxStackPush(u32);
void VuMtxMul(s32, s32, void *);

void VwrBuildDrawLists(s32 arg0) {
    Tmp t;
    VuVec4 rot;
    f32 dir[4];
    GphLight *l;
    GphModel *o;
    u32 i;

    VwrViewSetup(arg0);
    if (vwrCamera.yaw < 0.0f) {
        vwrCamera.yaw += 6.283184f;
    } else if (vwrCamera.yaw >= 6.283184f) {
        vwrCamera.yaw -= 6.283184f;
    }
    if (vwrCamera.pitch < 0.0f) {
        vwrCamera.pitch += 6.283184f;
    } else if (vwrCamera.pitch >= 6.283184f) {
        vwrCamera.pitch -= 6.283184f;
    }
    if (sysIni.viewerNoLight == 0) {
        DbgLightAngleGet(0, &rot);
        rot.x += vwrCamera.pitch * 57.295776f;
        rot.y += vwrCamera.yaw * 57.295776f;
        VuDirFromAngles(dir, &rot);
        l = GphLightGet(1);
        l->dir[0] = dir[0];
        l->dir[1] = dir[1];
        l->dir[2] = dir[2];
        l->dir[3] = 0.0f;
    }
    DbgLightAngleGet(1, &rot);
    VuDirFromAngles(dir, &rot);
    GphShadowCalcMtx(vwrShadowMtx, dir);
    GphDrawListClear(0);
    VuMtxStackReset();
    VuMtxStackLoad(&vwrCamera);
    GphLightListUpdate();
    for (i = 0; i < vwrModelList.count; i++) {
        if (vwrModelList.count - 1 == i && dbgMotObjParams[2].flag == 0) {
            continue;
        }
        if (vwrModelList.count - 2 == i && dbgMotObjParams[0].flag == 0) {
            continue;
        }
        o = vwrModelList.items[i];
        if (o != NULL) {
            copyq(&t.pos, &o->pos);
            t.x10 = o->rot[0];
            t.x14 = o->rot[1];
            t.x18 = o->rot[2];
            GphModelCalc(o);
            GphModelDraw(o);
        }
    }
    PtclDrawModels(0);
    GphDrawListSort();
    GphDrawListEnvMapCalcUv();
    GphDrawListSet(vwrWork.shadowList);
    GphDrawListClear(0);
    if (dbgMotObjParams[1].flag != 0) {
        s32 r;
        VuMtxStackPush(0);
        r = VuMtxStackGetTop();
        VuMtxMul(r, r, vwrShadowMtx);
        for (i = 0; i < vwrModelList.count - 2; i++) {
            o = vwrModelList.items[i];
            if (o != NULL) {
                GphModelDrawSimple(o);
            }
        }
        GphDrawListSort();
        VuMtxStackPop();
    }
}
