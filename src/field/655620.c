/* rodata: 0x006758C0-0x00675940 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

extern f32 VuVecDot(f32 *, f32 *);
extern s32 wmapDbgCamEdit;
extern s32 wmapDbgCamMode;

/* byte-offset node address: nodes[idx] is 21 words off (operand order and the shared idx * 0x50) */
#define ENT(o) ((WmapNode *)((u8 *)wmapWork + (o)))

void WmapDebugDrawCam(void) {
    FVECTOR v;
    FVECTOR w;
    Pad *pad;
    f32 dy;
    f32 dz;
    f32 dx;
    s32 idx;
    s32 off;
    f32 d;

    if (wmapWork->active != 0) {
        if (SysIsDebugMode() != 0) {
            DbgPrintf(2, 4, " map_no : %04x  (idx: %2d)", wmapWork->nodes[wmapWork->cur].mapNo, wmapWork->cur);
            pad = &padPort1;
            if (((pad - 1)->trigger & 0x50) == 0x50) {
                wmapDbgCamEdit ^= 1;
            }
            if (wmapDbgCamEdit != 0) {
                dy = 0x80 - pad->leftX;
                dz = pad->leftY - 0x80;
                dx = pad->rightY - 0x80;
                idx = wmapWork->cur;
                if (pad->trigger & 0x5000) {
                    wmapDbgCamMode ^= 1;
                }
                switch (wmapDbgCamMode) {
                case 0: {
                    s32 o = idx * 0x50;
                    copyq(v, ENT(o)->camTarget);
                    if (fabsf(dy) > 40.0f) {
                        v[0] += dy * 0.1f;
                    }
                    if (fabsf(dx) > 40.0f) {
                        v[2] += dx * 0.1f;
                    }
                    copyq(ENT(o)->camTarget, v);
                    copyq(wmapCamTargetGoal, v);
                    break;
                }
                case 1: {
                    s32 o = idx * 0x50;
                    copyq(w, ENT(o)->camPos);
                    if (fabsf(dy) > 40.0f) {
                        w[0] += dy * 0.1f;
                    }
                    if (fabsf(dz) > 40.0f) {
                        w[1] += dz * 0.1f;
                    }
                    if (fabsf(dx) > 40.0f) {
                        w[2] += dx * 0.1f;
                    }
                    copyq(ENT(o)->camPos, w);
                    copyq(wmapCamPosGoal, w);
                    break;
                }
                }
                DbgPuts(1, wmapDbgCamMode + 5, "> ");
                off = idx * 0x50;
                copyq(v, ENT(off)->camTarget);
                DbgPrintf(2, 5, "cam_target: (%7.2f, %7.2f, %7.2f) ", v[0], v[1], v[2]);
                copyq(w, ENT(off)->camPos);
                DbgPrintf(2, 6, "cam_pos: (%7.2f, %7.2f, %7.2f) ", w[0], w[1], w[2]);
                __asm__ volatile(
                    "lqc2 $vf1, 0(%1)\n\t"
                    "lqc2 $vf2, 0(%2)\n\t"
                    "vsub.xyz $vf1, $vf1, $vf2\n\t"
                    "sqc2 $vf1, 0(%0)"
                    : : "r"(v), "r"(v), "r"(w) : "memory");
                d = fsqrt(VuVecDot(v, v));
                DbgPrintf(2, 7, "distance: %7.2f", d);
            }
        }
    }
}
