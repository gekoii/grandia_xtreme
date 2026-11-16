#include "common.h"
#include "main/n07.h"

void GphFadeSetColor(GphFade *arg0, f32 a, f32 b, f32 c, f32 d) {
    arg0->color[0] = a;
    arg0->color[1] = b;
    arg0->color[2] = c;
    arg0->color[3] = d;
}
