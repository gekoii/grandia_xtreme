/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    s32 active;
    u8 pad4[0xC];
    u8 pos[0x10];
} WmapEnterFx;

extern WmapEnterFx wmapEnterFx;
extern s32 wmapEnterTimer;

void WmapEnterUpdate(void) {
    f32 a;
    WmapNode *e;

    if (EvtScnrIsRunning() == 1) {
        return;
    }
    wmapRipples->period = 600;
    a = wmapWork->warpT;
    if (wmapEntering == 0) {
        wmapEntering = 1;
        wmapEnterFx.active = 1;
        a = 0.0f;
        copyq(wmapEnterFx.pos, wmapCursorGoal);
        wmapEnterTimer = 0;
        SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        GphFadeSetColor(fldFades, a, a, a, a);
        GphFadeStart(fldFades, 0, a, a, a, 128.0f, 120.0f);
    }
    if (wmapEnterTimer >= 120) {
        e = &wmapWork->nodes[wmapWork->cur];
        MapJumpRequest(e->mapNo, e->entrance);
        return;
    }
    WmapWarpCalcGrid(a, 320.0f, 224.0f, 100.0f);
    a += 0.37699103f;
    if (a > 62.83184f) {
        a = 62.83184f;
    }
    wmapWork->warpT = a;
    wmapEnterTimer++;
    GphLightGet(1);
}
