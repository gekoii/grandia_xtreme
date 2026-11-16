#include "common.h"
#include "sys.h"



void SprSetColor(u8 arg0, u8 arg1, u8 arg2) {
    SprState *p = &sprState;
    p->col[0].r = arg0;
    p->col[0].g = arg1;
    p->col[0].b = arg2;
}
