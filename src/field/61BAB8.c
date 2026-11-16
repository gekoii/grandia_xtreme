/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n22.h"

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

s32 FldSpotFindNear(void *p)
{
    VuVec4 d __attribute__((aligned(16)));
    FldMapSet *ev;
    f32 r;
    f32 best;
    f32 len;
    s32 found;
    u32 i;

    ev = MapSetFileGet();
    if (ev == NULL) {
        return -1;
    }
    found = -1;
    r = ev->spotRange + 40.0f;
    best = 999999.0f;
    for (i = 0; i < fldSpotWork->count; i++) {
        if (fldSpotWork->list[i].unkA != 0) {
            __asm__ volatile(
                "lqc2 $vf1, 0(%1)\n\t"
                "lqc2 $vf2, 0(%2)\n\t"
                "vsub.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(&d), "r"(p), "r"(&fldSpotWork->list[i].model->pos) : "memory");
            len = fsqrt(d.x * d.x + d.y * d.y + d.z * d.z);
            if (len < r && len < best) {
                best = len;
                found = i;
            }
        }
    }
    return found;
}
