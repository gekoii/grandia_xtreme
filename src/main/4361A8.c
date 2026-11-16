/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "main/n04.h"

typedef struct {
    f32 x;
    f32 y;
} Key;

void GphMotKeyUvOffset(GphPart *arg0, GphMotKeys *arg1, f32 time) {
    Key *keys;
    u16 *tm;
    s32 i;
    f32 t0;
    Key *k;
    Key *k2;
    f32 f;
    f32 fy;
    s32 j;
    f32 fx;

    keys = arg1->values;
    if (arg1->count == 1) {
        fx = keys[0].x;
        fy = keys[0].y;
        arg0->uvScroll[2] = (s32)(fy * 4096.0f) & 0xFFF;
        arg0->uvScroll[0] = (s32)(fx * 4096.0f) & 0xFFF;
    } else {
        tm = arg1->frames;
        i = time;
        i = arg1->frameIndex[i];
        t0 = tm[i];
        if (t0 == time) {
            k = &keys[i];
            fx = k->x;
            fy = k->y;
            arg0->uvScroll[2] = (s32)(fy * 4096.0f) & 0xFFF;
            arg0->uvScroll[0] = (s32)(fx * 4096.0f) & 0xFFF;
        } else {
            j = i + 1;
            f = (time - t0) / (tm[j] - t0);
            k = &keys[i];
            k2 = &keys[j];
            fx = k->x * (1.0f - f) + k2->x * f;
            fy = k->y * (1.0f - f) + k2->y * f;
            arg0->uvScroll[2] = (s32)(fy * 4096.0f) & 0xFFF;
            arg0->uvScroll[0] = (s32)(fx * 4096.0f) & 0xFFF;
        }
    }
}
