#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"
#include "main/n05.h"


static inline void qcopy(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

GphModel *ChrModelCreate(ChrModelData *src) {
    GphModel *obj;
    u8 *p;
    GphModelMotion *sub;
    void **q;               /* &chrModelCreateGaObjPtr */
    void *t;
    s32 n1;

    obj = ChrModelPoolAlloc(0x90);
    obj->flags = src->flags;
    obj->objId = src->unk64;
    chrModelCreated = obj;
    obj->objId = src->unk60;
    obj->pktSize = 0;
    obj->gaObjCount = src->gaObjCount;
    qcopy(obj->pos, src->pos);
    qcopy(obj->rot, src->rot);
    qcopy(obj->scale, src->scale);
    obj->motionMode = 0;
    n1 = src->partCount + 1;
    sub = (GphModelMotion *)&obj->motionMode;
    sub->slot[0].motion = NULL;
    sub->slot[0].frame = 0;
    sub->slot[1].motion = NULL;
    sub->slot[1].frame = 0;
    chrModelCreateUnused0 = 0;
    chrModelCreateUnused2 = 0;
    chrModelCreateUnused1 = 0;
    chrModelCreatePktSize = 0;
    q = (void **)&chrModelCreateGaObjPtr;
    p = ChrModelPoolAlloc(n1 << 6);
    obj->mtx = (f32 *)p;
    obj->restMtx = (f32 *)(p + 0x40);
    chrModelCreateMtxPtr = (f32 *)(p + 0x40);
    chrModelCreatePartPtr = ChrModelPoolAlloc(src->partCount * 0xA0);
    t = ChrModelPoolAlloc(src->gaObjCount << 7);
    obj->partCount = src->partCount;
    *q = t;
    /* *q and this store go through void * (one alias set): the scheduling needs it */
    *(void **)&obj->parts = chrModelCreatePartPtr;
    obj->gaObjs = t;
    chrModelCreateHasDeform = 0;
    ChrModelCreateParts(src->parts, src->partCount);
    if (chrModelCreateHasDeform != 0) {
        obj->unkB |= 1;
    }
    qcopy(obj->cullSphere, src->cullSphere);
    obj->pktSize = chrModelCreatePktSize;
    obj->cullScale = 2.0f;
    return obj;
}
