/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00674E88-0x00674E98 */
#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "gph.h"
#include "field/state.h"


#define NORM(x)                     \
    while ((x) > 360.0f) {          \
        (x) -= 360.0f;              \
    }                               \
    while ((x) < -360.0f) {         \
        (x) += 360.0f;              \
    }

void FldBattleRollInitiative(void) {
    u8 tbl[3] = { 2, 0, 1 };
    f32 lim[2] = { 60.0f, 90.0f };
    ChrObj *a;
    ChrObj *b;
    s32 k;
    f32 dir;
    f32 rot;
    f32 d1;
    f32 d2;
    s32 r;
    u32 f;
    u32 g;

    a = ChrGetByNo(1);
    b = ChrTenantGet(btlHandoff.unk50);
    f = a->actFlags;
    if (f & 8) {
        g = f & 0x10;
        k = !g;
    } else {
        k = 0;
    }
    dir = FldCalcYaw(b->model->pos, a->model->pos) * 0.0054931641f;
    rot = a->heading * 0.0054931641f;
    NORM(dir);
    NORM(rot);
    d1 = __builtin_fabsf(ChrAngleDiffDeg(rot, dir));
    dir = FldCalcYaw(a->model->pos, b->model->pos) * 0.0054931641f;
    rot = b->heading * 0.0054931641f;
    NORM(dir);
    NORM(rot);
    d2 = __builtin_fabsf(ChrAngleDiffDeg(rot, dir));
    r = VuRandInt(100);
    do { /* empty: scheduling barrier, the lim[k] load stays after the VuRandInt call and r keeps its register */
    } while (0);
    if (d1 < lim[k]) {
        if (d2 < 60.0f) {
            r = r < 70;
        } else {
            r = r < 90;
        }
    } else {
        if (d2 < 60.0f) {
            r = r < 10;
        } else {
            r = r < 30;
        }
    }
    r += k;
    btlHandoff.encType = tbl[r];
}
