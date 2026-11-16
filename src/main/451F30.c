/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n07.h"

s32 VuFloatToInt(f32);

void GphFadeStart(GphFade *p, u8 mode, f32 x, f32 y, f32 z, f32 w, f32 t) {
    p->mode = mode;
    p->count = 0;
    p->active = 1;
    p->time = t;
    p->unk3 = VuFloatToInt(w);
    if (mode == 0) {
        if (VuFloatToInt(p->color[3]) == 0) {
            p->color[0] = x;
            p->color[1] = y;
            p->color[2] = z;
        }
        p->unk2 = 0;
        p->delta[0] = (x - p->color[0]) / p->time;
        p->delta[1] = (y - p->color[1]) / p->time;
        p->delta[2] = (z - p->color[2]) / p->time;
        p->delta[3] = (w - p->color[3]) / p->time;
    }
}
