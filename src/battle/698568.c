#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "gph.h"


void FxCloneStart(BtlUnit *ent, s8 n) {
    FxClone *p = &fxClone;
    ChrModelData *res;
    s32 *h;
    GphModel *pt;
    s32 i;
    s32 j;
    s32 k;
    s32 r;
    s32 buf[8];

    res = ent->def->modelPack;
    if (p->unit != NULL || res == NULL) {
        return;
    }
    h = p->drawLists;
    p->unit = ent;
    p->circleMode = 0;
    p->pool = 0;
    p->drawList = 0;
    for (j = 0; j < 3; j++) {
        p->drawLists[j] = 0;
    }
    if (n >= 4) {
        n = 3;
    }
    p->shadowCb = -1;
    p->count = n;
    BtlModelPoolReserve((s32)res, n);
    p->pool = BtlModelPoolAlloc(buf);
    if (p->pool == 0) {
        FxCloneFree();
        return;
    }
    for (i = 0; i < n; i++) {
        pt = BtlModelCreate((s32)res, 0);
        p->clones[i].model = pt;
        p->clones[i].alpha = p->clones[i].target = 1.0f;
        pt->flags |= 0x200;
        p->clones[i].step = 0.0f;
    }
    ChrModelPoolSet(0);
    p->drawList = MemAlloc32(GphDrawListSize(res->gaObjCount) * n);
    if (p->drawList == 0) {
        FxCloneFree();
        return;
    }
    GphDrawListInit(res->gaObjCount * n, p->drawList);
    GphDrawListSet(p->drawList);
    GphDrawListClear(0);
    for (k = 0; k < n; k++) {
        r = MemAlloc32(GphDrawListSize(res->gaObjCount));
        h[k] = r;
        if (r == 0) {
            FxCloneFree();
            return;
        }
        GphDrawListInit(res->gaObjCount, r);
        GphDrawListSet(h[k]);
        GphDrawListClear(0);
    }
    p->shadowCb = EnvShadowAddCallback(FxCloneShadowBuild, FxCloneShadowDraw, p);
    p->unk4C = -1;
}
