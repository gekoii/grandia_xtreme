#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "main/n05.h"
#include "battle/n32.h"


void BtlUnitsCreateModels(s32 *out, BtlUnit *units, s32 n) {
    ChrModelPool st;
    BtlUnit *u;
    s32 i;
    s32 size;
    s32 vsize;
    BtlCharDef *obj;
    ChrModelData *res;

    vsize = 0;
    size = 0;
    for (i = n, u = units; i > 0; u++) {
        obj = u->def;
        i--;
        if (obj != NULL && (res = obj->modelPack) != NULL) {
            size += ChrModelCalcInstanceSize(res);
            vsize += u->def->modelPack->gaObjCount;
        }
    }
    i = n;
    u = units;
    ChrModelPoolInit(&st, MemAlloc128(size), size);
    ChrModelPoolSet(&st);
    *out = MemAlloc32(GphDrawListSize(vsize));
    GphDrawListInit(vsize, *out);
    GphDrawListSet(*out);
    GphDrawListClear(0);
    for (; i > 0; ) {
        obj = u->def;
        if (obj != NULL && (res = obj->modelPack) != NULL) {
            u->model = BtlModelCreate((s32)res, 2);
        } else {
            u->model = NULL;
        }
        u->motion = NULL;
        u->evtCur = NULL;
        u->motionLast = 0;
        i--;
        BtlChrSetMotion(u, 0);
        u++;
    }
}
