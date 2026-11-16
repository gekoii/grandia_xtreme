/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"
#include "sys.h"

#include "vu.h"

typedef struct {
    u8 pad0[0x60];
    FVECTOR eye;
    FVECTOR dir;
} Cam;

typedef struct {
    Cam *cam;
} Spr;



f32 VuVecNormalizeLen(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

#define VSUB(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

void SprDrawRectAtWorld(void *rect, s16 x, s16 y, f32 *pos)
{
    Spr *spr = (Spr *)0x70002700;
    FVECTOR scr;
    FVECTOR d;
    f32 len;
    f32 sx, sy, ox, oy;

    VSUB(d, pos, spr->cam->eye);
    len = VuVecNormalizeLen(d, d);
    len *= VuVecDot(d, spr->cam->dir);
    if (len < 10.0f) {
        return;
    }
    len = sprFocalLength / len;
    sx = sprState.sx;
    sy = sprState.sy;
    sprState.sx = sx * len;
    sprState.sy = sy * len;
    if (UtilWorldToScreen(scr, pos) != 0) {
        ox = sprState.ox;
        oy = sprState.oy;
        sprState.ox = scr[0];
        sprState.oy = scr[1];
        SprDrawRect(rect, x, y);
        sprState.sx = sx;
        sprState.ox = ox;
        sprState.oy = oy;
        sprState.sy = sy;
    }
}
