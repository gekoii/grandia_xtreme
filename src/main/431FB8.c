#include "common.h"
#include "gph.h"


void GphLightInit(GphLight *arg0) {
    arg0->unk0 = 0;
    arg0->unk2 = 0;
    arg0->id = 0;
    arg0->type = 0;
    arg0->enable = 0;
    arg0->color[0] = 128.0f;
    arg0->color[1] = 128.0f;
    arg0->color[2] = 128.0f;
    arg0->dir[1] = -1.0f;
    arg0->color[3] = 0;
    arg0->range = 1000.0f;
    arg0->dir[0] = 0;
    arg0->dir[2] = 0;
    arg0->dir[3] = 0;
    arg0->intensity = 1.0f;
}
