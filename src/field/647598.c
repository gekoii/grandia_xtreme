/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n24.h"

void VuVecCopy(void *, void *);

void MiniConcertCameraUpdate(void)
{
    FVECTOR v;
    FVECTOR tmp;
    MiniConcertPos *k;
    MiniConcertPos *e;
    f32 t;
    f32 d;
    s32 idx;
    u16 id;

    k = &miniConcertWork->cam;
    miniConcertWork->camFrame++;
    t = (f32)miniConcertWork->camFrame / (f32)k->frames;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(v), "r"(k->to), "r"(k->from) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "qmtc2.ni %2, $vf2\n\t"
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(tmp), "r"(v), "r"(t) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(v), "r"(tmp), "r"(k->from) : "memory");
    VuVecCopy(camField[0].lookAt, v);
    d = k->yawTo - k->yaw;
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    camField[0].dist = k->dist + (k->distTo - k->dist) * t;
    camField[0].yaw = (k->yaw + d * t) / 180.0f * 3.14159f;
    camField[0].pitch = (k->pitch + (k->pitchTo - k->pitch) * t) / 180.0f * 3.14159f;
    if (miniConcertWork->camFrame == k->frames) {
        idx = 0;
        do {
            idx = VuRandInt(miniConcertWork->camCount) + miniConcertWork->camFirst;
            e = &miniConcertData->pos[idx];
            id = e->id;
        } while (id == miniConcertWork->camId);
        *k = *e;
        miniConcertWork->camFrame = 0;
        miniConcertWork->camIdx = idx;
        miniConcertWork->camId = id;
    }
}
