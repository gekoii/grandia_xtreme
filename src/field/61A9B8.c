#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n22.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void MapNearObjAdd(s32 arg0, void *arg1, u8 arg2, f32 f) {
    u32 n;
    GphPart *o;
    MapNearObj *e;

    n = mapNearObjTable->count;
    if (n >= 0x20) {
        return;
    }
    mapNearObjTable->objs[n].inst = ChrInstSpawn(arg0, arg1, 0);
    mapNearObjTable->objs[n].partPos = (f32 *)ChrInstGetPos(arg0, mapNearObjTable->objs[n].inst);
    mapNearObjTable->objs[n].group = arg0;
    mapNearObjTable->objs[n].range = f;
    mapNearObjTable->objs[n].alpha = 0.0f;
    mapNearObjTable->objs[n].t = 0.0f;
    mapNearObjTable->objs[n].state = 1;
    mapNearObjTable->objs[n].id = arg2;
    ChrInstSetVisible(arg0, mapNearObjTable->objs[n].inst, 0);
    ChrInstSetSpin(arg0, mapNearObjTable->objs[n].inst);
    o = ChrModelFindPart(ChrInstGetModel(arg0, mapNearObjTable->objs[n].inst), 1);
    o->flags = 0;
    if (o != NULL) {
        e = &mapNearObjTable->objs[n];
        copyq(e, o->pos);
    }
    mapNearObjTable->count++;
}
