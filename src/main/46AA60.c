#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

u32 *MovGsSpriteFlat(u32 *arg0, MovRect *r, u32 c) {
    return MovGsXyz2(
            MovGsRgbaq(
                MovGsXyz2(MovGsPrim(arg0, 6, 0, 0, 0, 0, 0, 1, 0, 0), r->x, r->y, 0),
                c & 0xFF, (c >> 8) & 0xFF, (c >> 16) & 0xFF, c >> 24, 0),
            r->x + r->w, r->y + r->h, 0);
}
