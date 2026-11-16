/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/n25.h"

extern f32 wmapUnusedLevel;

s32 WmapReadDir(Pad *);
f32 VuVecDot(f32 *, f32 *);
void WmapVecApproach(void *, void *, void *, f32, f32);

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define vsub(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void WmapControl(void)
{
    FVECTOR d;
    u8 *obj;
    s32 k;
    s16 n;
    WmapNode *e;
    u8 t;
    GphPart *o;
    WmapRipples *fx;

    obj = MapModelGetObj(1);
    wmapRipples->period = 0xB4;
    wmapRipples->flags = 0;
    if ((padPort0[0].trigger & 0x20) && EvtScnrIsRunning() != (t = 1)) {
        if (WmapCheckEvent() == 2) {
            return;
        }
        vsub(d, wmapCursorPos, wmapCursorGoal);
        if (!(fsqrt(VuVecDot(d, d)) > 100.0f)) {
            wmapWork->mode = t;
            copyq(wmapWork->markerPos, obj + 0x10);
            wmapUnusedLevel = 128.0f;
            FldMenuWinCloseAll();
            return;
        }
    }
    /* do-while(0): loop-depth weight that sets the s-register order in this block */
    do {
        if (EvtScnrIsRunning() != 0 || wmapWork->eventDone != 0) {
            goto join;
        }
        k = WmapReadDir(padPort0);
        if (k == -1) {
            goto join;
        }
        e = &wmapWork->nodes[wmapWork->cur];
        n = e->links[k];
        if (n == 0xFF) {
            goto join;
        }
        e = &wmapWork->nodes[n];
        if (e->unlocked == 0) {
            goto join;
        }
        t = e->state;
        if (t != 1) {
            goto join;
        }
        wmapWork->cur = n;
        copyq(wmapCursorGoal, e->pos);
        copyq(wmapCamPosGoal, wmapWork->nodes[n].camPos);
        copyq(wmapCamTargetGoal, wmapWork->nodes[n].camTarget);
        o = ChrModelGetPart(FldObjFindById(n + 6), 0);
        o->color[0] = 128.0f;
        o->color[1] = 128.0f;
        o->color[2] = 0.0f;
        fx = wmapRipples;
        fx->flags = t;
        fx->timer = 0;
        SndSePlay(0x7D1, 0, 0, 0x1E, 0);
    } while (0);
join:
    WmapVecApproach(wmapWork->cursorVel, wmapCursorPos, wmapCursorGoal, 80.0f, 4.0f);
}
