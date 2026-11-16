#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "gph.h"

f32 VuSinAngle(s32);
void GphModelCalcCull(GphModel *, f32);

void FxAfterimageBuild(void) {
    FxAfterimage *p = &fxAfterimage;
    FxAfterimageSnap *q;
    GphModel *s;
    GphModel *s2;
    FxAfterimagePose *t;
    s32 i;
    s32 k;
    s32 j;
    f32 f;
    s32 c;

    if (p->unit != NULL && p->count > 0) {
        for (i = 0; i < (c = p->count); i = k) {
            k = i + 1;
            j = p->cur - k * p->lag / p->count;
            if (j < 0) {
                j += p->length;
            }
            f = k;
            s = p->unit->model;
            t = &p->poses[j];
            s->motion[0].motion = t->motion;
            ChrModelClearDeformBufs(s);
            s->pos[0] = t->pos[0] + p->spread * VuSinAngle(0x8000 - t->rot[1]) * f;
            s->pos[1] = t->pos[1];
            s->pos[2] = t->pos[2] + p->spread * VuCosAngle(0x8000 - t->rot[1]) * f;
            s->rot[0] = t->rot[0];
            s->rot[1] = t->rot[1];
            s->rot[2] = t->rot[2];
            s->scale[0] = t->scale[0];
            s->scale[1] = t->scale[1];
            s->scale[2] = t->scale[2];
            s->motion[0].frame = t->frame;
            GphModelCalcCull(s, 0.0f);
            GphDrawListSet(p->drawLists[i]);
            GphDrawListClear(0);
            GphModelDrawCopy((s32)s);
            GphDrawListIndex();
            c = p->count;
        }
        BtlChrModelInvalidate(p->unit->id);
        if (p->unit->model != NULL) {
            BtlChrModelUpdate(p->unit);
        }
    }
    q = &fxAfterimageSnap;
    if (q->motion != 0) {
        s2 = q->unit->model;
        s2->motion[0].motion = q->motion;
        ChrModelClearDeformBufs(s2);
        s2->pos[0] = q->pos[0];
        s2->rot[0] = q->rot[0];
        s2->pos[1] = q->pos[1];
        s2->rot[1] = q->rot[1];
        s2->pos[2] = q->pos[2];
        s2->rot[2] = q->rot[2];
        s2->scale[0] = q->scale[0];
        s2->scale[1] = q->scale[1];
        s2->scale[2] = q->scale[2];
        s2->motion[0].frame = q->frame;
        GphModelCalcCull(s2, 0.0f);
        GphDrawListSet(fxAfterimageSnapDrawList);
        GphDrawListClear(0);
        GphModelDrawCopy((s32)s2);
        GphDrawListIndex();
        BtlChrModelInvalidate(q->unit->id);
        if (q->unit->model != NULL) {
            BtlChrModelUpdate(q->unit);
        }
    }
}
