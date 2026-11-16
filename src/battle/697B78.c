#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAfterimageStart(BtlUnit *ent, s32 n, s32 cnt) {
    FxAfterimage *p = &fxAfterimage;
    s32 *h;
    s32 *hs;
    s32 i;
    u32 size;
    s32 r;

    if (p->unit != NULL) {
        return;
    }
    p->poses = MemAlloc16A(n * 0x2C);
    if (p->poses == NULL) {
        return;
    }
    if (cnt >= 9) {
        cnt = 8;
    }
    size = ent->def->modelPack->gaObjCount;
    for (i = 0; i < cnt; i++) {
        hs = p->drawLists;
        r = MemAlloc32(GphDrawListSize(size));
        h = &hs[i];
        *h = r;
        if (r == 0) {
            FxAfterimageFree();
            return;
        }
        GphDrawListInit(size, r);
        GphDrawListSet(*h);
        GphDrawListClear(0);
    }
    p->unit = ent;
    p->length = n;
    p->count = cnt;
    p->color0[3] = 76.8f;
    p->color0[1] = 64.0f;
    p->color0[2] = 102.4f;
    p->color1[3] = 25.6f;
    p->cur = 0;
    p->lag = 0;
    p->full = 0;
    p->spread = 0;
    p->color0[0] = 38.4f;
    p->color1[0] = 0;
    p->color1[1] = 0;
    p->color1[2] = 38.4f;
}
