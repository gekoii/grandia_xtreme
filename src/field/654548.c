/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

extern void VuVecCopy(void *, void *);
extern void WmapPathEval(void *, void *, s32, f32);
extern f32 wmapPathTime;
extern u8 wmapPathKeys[];
extern s32 wmapCamFrame;

void WmapUpdate(void) {
    u8 *obj;
    GphPart *n;
    GphLight *l;
    s32 i;
    u32 cnt;
    s32 mode;
    f32 hi;
    WmapWork *st;

    i = 0;
    st = wmapWork;
    cnt = wmapWork->frame;
    if (st->active == 0) {
        return;
    }
    /* do-while(0): extra ref weight that gives obj and i their s registers */
    do {
        obj = MapModelGetObj(1);
    } while (0);
    if (obj == NULL) {
        return;
    }
    hi = 128.0f;
    VuVecCopy(obj + 0x10, &wmapCursorPos);
    do {
        obj = FldObjFindById(i + 6);
        if (obj != NULL) {
            n = ChrModelGetPart(obj, 0);
            if (wmapWork->cur == i) {
                n->color[2] = -128.0f;
            } else {
                n->color[2] = hi;
            }
            n->color[1] = hi;
            n->color[0] = hi;
        }
        i++;
    } while (i < 6);
    obj = FldObjFindById(2);
    n = ChrModelGetPart(obj, 0);
    if (n != NULL) {
        n->flags &= 0xFFFE;
        WmapPathEval(n->pos, wmapPathKeys, 0xB, wmapPathTime);
    }
    wmapPathTime += 1.0f;
    if (wmapPathTime >= 160.0f) {
        wmapPathTime = 0.0f;
    }
    l = GphLightGet(3);
    l->dir[0] = wmapCursorPos[0];
    l->dir[1] = wmapCursorPos[1] + 200.0f;
    l->dir[2] = wmapCursorPos[2];
    l->dir[3] = 1.0f;
    if (cnt >= 61) {
        switch (wmapWork->mode) {
        case 0:
            WmapControl();
            break;
        case 1:
            WmapEnterUpdate();
            break;
        }
    }
    wmapWork->waveT += 2.0f;
    if (wmapWork->waveT >= 360.0f) {
        wmapWork->waveT = 0.0f;
    }
    if (cnt < 61) {
        wmapCamFrame = cnt;
        mode = 0;
    } else {
        goto other;
    /* goto-built layout: zero and call are shared exits, entered from the switch below as in the original */
    zero:
        wmapCamFrame = 0;
        goto call;
        /* do-while(0): keeps the branch layout of the goto-entered switch */
        do {
    other:
        switch (wmapWork->mode) {
        case 1:
            mode = 2;
            if (wmapEntering == 0) {
                goto zero;
            }
            wmapCamFrame++;
            break;
        default:
            mode = 1;
            break;
        }
        } while (0);
    }
call:
    WmapCamUpdate(mode, wmapCamFrame);
    copyq(wmapWork->markerPos, obj + 0x10);
    if (cnt >= 61 && wmapWork->mode == 2) {
        WmapRippleUpdate();
    }
    WmapNodesMarkLinked();
    wmapWork->unk29C += 1.0f;
    if (wmapWork->unk29C >= 360.0f) {
        wmapWork->unk29C = 0.0f;
    }
    wmapWork->frame = cnt + 1;
}
