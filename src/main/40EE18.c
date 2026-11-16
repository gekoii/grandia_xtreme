#include "common.h"
#include "sys.h"



void SprBegin(f32 arg0, f32 arg1) {
    sprState.sy = 1.0f;
    sprState.angle = 0;
    sprState.flags = 0;
    sprState.col[0].b = 0x80;
    sprState.col[0].g = 0x80;
    sprState.col[0].r = 0x80;
    sprState.offY = 0;
    sprState.offX = 0;
    sprState.skewY = 0;
    sprQuadCount = 0;
    sprRectCount = 0;
    sprQuad3DCount = 0;
    sprState.sx = 1.0f;
    sprState.col[0].a = 0x80;
    sprState.blend = 1;
    sprState.ox = arg0;
    sprState.oy = arg1;
    sprState.skewX = 0;
}
