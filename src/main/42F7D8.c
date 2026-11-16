/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphViewSetFov(s32 arg0, f32 fov) {
    f32 t;
    GphView *c;

    t = 320.0f / tanf(((fov * 3.141592f) / 180.0f) * 0.5f);
    c = &gphViews[arg0];
    c->fov = fov;
    c->projDist = t;
}
