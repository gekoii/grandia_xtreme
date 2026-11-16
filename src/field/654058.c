/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/state.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 padA[6];
    f32 unk10;
    f32 unk14;
    f32 unk18;
} Obj;

typedef struct {
    u8 pad0[0xC];
    u16 unkC;
} Chara;

extern void *FldMemAlloc(s32);
extern void VuVecCopy(void *, void *);
extern void CamCalcMatrix(Camera *);
extern f32 wmapUnusedLevel;

void WmapStart(s32 arg0) {
    Chara *c;
    void *o;
    void *p;
    GphLight *l;
    Obj *ob;
    s32 i;
    u16 id;
    f32 *src;
    WmapNode *ent;
    WmapWork *st;
    void **q4;
    void **q8;

    fldState.mode = 2;
    c = ChrGetByNo(1);
    c->unkC ^= 0xC;
    MapModelSetVisible(2, 0);
    MapModelSetVisible(3, 0);
    ChrAllDeactivate();
    FldObjGet(0);
    camCtl.prevType = 13;
    camCtl.aiType = CAM_AI_NULL;
    st = wmapWork;
    i = 1;
    st->active = i;
    wmapWork->mode = 0;
    st->waveT = 0.0f;
    st->unk2E0 = 0.0f;
    wmapEntering = 0;
    wmapWork->unk290 = 100.0f;
    wmapWork->unk294 = 150.0f;
    wmapWork->unk298 = 60.0f;
    wmapWork->unk2A4 = 6.0f;
    wmapWork->unk29C = 0.0f;
    wmapWork->unk2A0 = 0.0f;
    wmapWork->warpT = 0.0f;
    wmapWork->frame = 0;
    o = MapModelGetObj(0);
    p = FldMemAlloc(ChrModelCalcVtxBufSize(o, 7));
    if (p != NULL) {
        ChrModelAssignVtxBufs(o, p, 7);
    }
    q4 = &wmapWarpGrid;
    q8 = &wmapWarpPacket;
    WmapRippleInit();
    wmapRipples->flags = i;
    i = 0;
    l = GphLightGet(3);
    l->dir[0] = wmapCursorPos[0];
    l->dir[1] = wmapCursorPos[1] + 1000.0f;
    l->dir[2] = wmapCursorPos[2];
    l->range = 1000.0f;
    l->dir[3] = 1.0f;
    l->intensity = 1.0f;
    l->enable = 0;
    l = GphLightGet(0);
    l->color[3] = 128.0f;
    l->color[1] = 80.0f;
    l->color[2] = 80.0f;
    l->color[0] = 80.0f;
    l = GphLightGet(1);
    l->enable = 1;
    l->intensity = 1.0f;
    l->color[0] = 80.0f;
    l->color[1] = 80.0f;
    l->color[3] = 0.0f;
    l->color[2] = 80.0f;
    *q4 = FldMemAlloc(0x8820);
    *q8 = FldMemAlloc(0x19E0);
    id = sysSaveData.oldMapNo;
    wmapWork->cur = 0;
    for (; i < 6; i++) {
        if (wmapWork->nodes[i].mapNo == id) {
            wmapWork->cur = i;
            break;
        }
    }
    ent = &wmapWork->nodes[wmapWork->cur];
    camField[0].mode = 0x23;
    camField[0].roll = 0;
    VuVecCopy(&wmapCursorPos, ent->pos);
    VuVecCopy(wmapCursorGoal, &wmapCursorPos);
    VuVecCopy(wmapCamPosGoal, wmapWork->nodes[wmapWork->cur].camPos);
    VuVecCopy(wmapCamTargetGoal, wmapWork->nodes[wmapWork->cur].camTarget);
    VuVecCopy(wmapCamTarget, &wmapCursorPos);
    VuVecCopy(&wmapCamPos, &wmapCursorPos);
    wmapCamPos[0] += 0.0f;
    wmapCamPos[1] += 1000.0f;
    wmapCamPos[2] -= 1000.0f;
    VuVecCopy(camField[0].lookAt, wmapCamTarget);
    VuVecCopy(camField[0].eye, &wmapCamPos);
    CamCalcMatrix(&camField[0]);
    VuVecCopy(wmapCursorTrail[0], &wmapCursorPos);
    VuVecCopy(wmapCursorTrail[1], &wmapCursorPos);
    VuVecCopy(wmapCursorTrail[2], &wmapCursorPos);
    VuVecCopy(wmapCursorTrail[3], &wmapCursorPos);
    wmapUnusedLevel = 128.0f;
    ob = MapModelGetObj(4);
    if (ob != NULL) {
        VuVecCopy(&ob->unk10, &wmapCursorPos);
        ob->unk14 -= 50.0f;
    }
    GphFadeSetColor(fldFades, 0.0f, 0.0f, 0.0f, 128.0f);
    GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f);
    WmapUnlockByProgress();
    for (i = 0; i < 6; i++) {
        ob = FldObjFindById(i + 6);
        if (ob != NULL) {
            src = wmapWork->nodes[i].pos;
            copyq(&ob->unk10, src);
            ob->unk14 += 100.0f;
            ob->unk18 -= 200.0f;
            ob->unk8 &= 0xFFFE;
            if (wmapWork->nodes[i].unlocked == 0) {
                ob->unk8 |= 8;
            }
        }
    }
}
