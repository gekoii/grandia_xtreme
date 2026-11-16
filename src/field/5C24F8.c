/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"

extern f32 chrTenantJumpStepLarge;
extern f32 chrTenantJumpStepMedium;
extern f32 chrTenantJumpStepSmall;
extern f32 chrTenantJumpHeightLarge;
extern f32 chrTenantJumpHeightMedium;
extern f32 chrTenantJumpHeightSmall;

void VuBezier3(f32 *, f32 *, f32);
void VuVecLerp(f32 *, f32 *, f32 *, f32);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

s32 ChrTenantOpJumpToPoint(void *arg0, ChrObj *a, s32 n, s32 lv)
{
    FVECTOR pts[4];
    FVECTOR v;
    FVECTOR buf;
    s32 ret;

    if (a->opState != 4) {
        a->opState = 4;
        switch (n) {
        case 7:
            n = VuRandInt(a->areas[a->area]->pointCount) + 1;
            break;
        case 8:
            n = ChrTenantFindAreaPoint(a, buf, 0);
            break;
        case 9:
            /* s32 cast needed: with the header prototype this function is one insn shorter (38+ words off) */
            n = ((s32 (*)(ChrObj *, void *, s32))ChrTenantFindAreaPoint)(a, buf, 1);
            break;
        }
        ChrTenantAreaGetPoint(a, n, a->moveTarget);
        if (a->moveTarget[0] == a->model->pos[0] && a->moveTarget[1] == a->model->pos[1] && a->moveTarget[2] == a->model->pos[2]) {
            return 0;
        }
        copyq(a->unk190, a->model->pos);
        a->unk40 = 0.0f;
        if (lv >= 11) {
            a->unk44 = chrTenantJumpStepLarge;
        } else if (lv >= 6) {
            a->unk44 = chrTenantJumpStepMedium;
        } else {
            a->unk44 = chrTenantJumpStepSmall;
        }
        if (a->flags & 8) {
            a->flags ^= 8;
        }
        a->flags |= 0x40;
        ChrMotSet(a, 1);
        ret = 1;
    } else {
        copyq(pts[0], a->unk190);
        copyq(pts[3], a->moveTarget);
        VuVecLerp(pts[1], a->unk190, a->moveTarget, 0.5f);
        copyq(pts[2], pts[1]);
        if (lv >= 11) {
            f32 k = lv * chrTenantJumpHeightLarge;
            pts[1][1] += k;
            pts[2][1] += k;
        } else if (lv >= 6) {
            f32 k = lv * chrTenantJumpHeightMedium;
            pts[1][1] += k;
            pts[2][1] += k;
        } else {
            f32 k = lv * chrTenantJumpHeightSmall;
            pts[1][1] += k;
            pts[2][1] += k;
        }
        a->unk40 += a->unk44;
        if (a->unk40 >= 1.0f) {
            copyq(a->model->pos, a->moveTarget);
            ret = 0;
            a->flags = (a->flags | 8) ^ 0x40;
        } else {
            VuBezier3(v, pts[0], a->unk40);
            if (FldVecDist(v, a->model->pos) > 10.0f) {
                a->heading = FldCalcYaw(v, a->model->pos);
            }
            copyq(a->model->pos, v);
            ret = 1;
        }
    }
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(a->vel) : "memory");
    return ret;
}
