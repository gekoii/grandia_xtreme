#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

u32 *MovGsSpriteTexBlend(u32 *arg0, MovRect *uv, MovRect *xy) {
    arg0 = MovGsPrim(arg0, 6, 0, 1, 0, 1, 0, 1, 0, 0);
    arg0 = MovGsUv(arg0, xy->x, xy->y);
    arg0 = MovGsXyz2(arg0, uv->x, uv->y, 0);
    arg0 = MovGsUv(arg0, xy->x + xy->w, xy->y + xy->h);
    return MovGsXyz2(arg0, uv->x + uv->w, uv->y + uv->h, 0);
}
