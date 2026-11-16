#include "common.h"
#include "sys.h"



void VuMtxSetRotZ(f32 (*)[4], s32);

void SprPushTransform(s32 arg0, f32 sx, f32 sy, f32 mx, f32 my) {
    f32 m[4][4];
    f32 x;
    f32 y;

    sprSavedState.ox = sprState.ox;
    sprSavedState.oy = sprState.oy;
    VuMtxSetRotZ(m, sprState.angle);
    x = sx * sprState.sx;
    y = sy * sprState.sy;
    sprSavedState.sx = sprState.sx;
    sprSavedState.sy = sprState.sy;
    sprSavedState.angle = sprState.angle;
    sprState.sx *= mx;
    sprState.sy *= my;
    sprState.ox += m[0][0] * x + m[1][0] * y;
    sprState.oy += m[0][1] * x + m[1][1] * y;
    sprState.angle += arg0;
}
