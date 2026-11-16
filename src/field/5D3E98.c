/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n16.h"

static inline FldTreasure *ent(u32 i) {
    return &fldTreasureWork[i];
}

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}
void VuVecCopy(void *, void *);

void FldTreasureSpawn(u16 arg0, VuVec4 *pos, u16 model, u8 id, f32 ang, s32 arg4) {
    u8 buf[32];
    VuVec4 v __attribute__((aligned(16)));
    u32 idx;
    u32 c;
    s32 r;

    copyq(&v, pos);
    c = ((FldTreasureWork *)fldTreasureWork)->count;
    if (c < 64) {
        ((FldTreasureWork *)fldTreasureWork)->count = c + 1;
    }
    idx = ((FldTreasureWork *)fldTreasureWork)->count;
    ItemInfoFill(buf, model, 0);
    { FldTreasure *e = &fldTreasureWork[idx];
    e->id = id; }
    { FldTreasure *e = &fldTreasureWork[idx];
    e->unk86 = arg0;
    e->flagId = arg4; }
    { FldTreasure *e = &fldTreasureWork[idx];
    e->item = model;
    e->unk7A = (s8)buf[2];
    e->t = 0.0f; }
    { u8 t = ItemGetPickupModel(*(u16 *)buf);
    FldTreasure *e = &fldTreasureWork[idx];
    e->instGroup = t; }
    { FldTreasure *e = &fldTreasureWork[idx];
    e->fallStep = VuRandSigned(0.1f) < 0.0f ? -VuRandSigned(0.1f) : VuRandSigned(0.1f); }
    { FldTreasure *e = &fldTreasureWork[idx];
    e->active = 1; }
    { FldTreasure *e = &fldTreasureWork[idx];
    if (e->fallStep < 0.04f) {
        e->fallStep = 0.04f;
    } }
    VuVecCopy(&fldTreasureWork[idx].path[0], &v);
    VuVecCopy(&fldTreasureWork[idx].path[1], &v);
    VuVecCopy(&fldTreasureWork[idx].path[2], &v);
    VuVecCopy(&fldTreasureWork[idx].path[3], &v);
    { FldTreasure *e = &fldTreasureWork[idx];
    e->state = 1; }
    { FldTreasure *e = &fldTreasureWork[idx];
    if (e->path[3][1] < 0.0f) {
        e->path[3][1] = 0.0f;
    } }
    { FldTreasure *e = &fldTreasureWork[idx];
    e->path[1][0] += 0.0f;
    e->path[1][2] += 200.0f;
    e->path[1][1] += 0.0f;
    e->path[2][1] += 300.0f;
    e->path[2][0] = e->path[3][0];
    e->path[2][2] = e->path[3][2];
    r = ChrInstSpawn(e->instGroup, &e->path[3], 0); }
    { FldTreasure *e = &fldTreasureWork[idx];
    e->inst = r;
    ChrInstSetSpin(e->unk7A, r); }
    { FldTreasure *e = &fldTreasureWork[idx];
    if (e->unk86) {
        ChrInstSetVisible(e->instGroup, e->inst, 0);
        r = ChrInstSpawn(0x4D, &fldTreasureWork[idx].path[3], 0);
        { FldTreasure *e2 = &fldTreasureWork[idx];
        e2->inst2 = r; }
        ((GphModel *)ChrInstGetModel(0x4D, r))->rot[1] = ang * 182.04445f;
    } }
}
