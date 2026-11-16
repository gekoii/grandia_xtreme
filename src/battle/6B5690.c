#include "common.h"
#include "protos/battle.h"
#include "chr.h"
#include "battle/unit.h"
#include "battle/env.h"


void EnvShadowInit(void) {
    BtlUnit *u;
    s32 *p;
    s32 i;
    BtlCharDef *obj;
    ChrModelData *res;
    s32 h;

    u = btlUnits;
    i = 15;
    p = envShadowDrawLists;
    do {
        obj = u->def;
        i--;
        u++;
        if (obj != NULL && (res = obj->modelPack) != NULL) {
            h = res->gaObjCount;
            *p = MemAlloc32(GphDrawListSize(h));
            GphDrawListInit(h, *p);
            GphDrawListSet(*p);
            GphDrawListClear(0);
        }
        p++;
    } while (i >= 0);
}
