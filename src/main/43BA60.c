#include "common.h"
#include "protos/main.h"
#include "main/n05.h"

void GphEnvMapSetTex(s32 arg0, s32 arg1) {
    GphEnvMap *e;

    e = &gphEnvMapTable[arg0];
    e->color[3] = 32.0f;
    e->tex = arg1;
    e->color[0] = 0;
    e->color[1] = 0;
    e->color[2] = 0;
    GsTexLoadPacket((s32)e, arg1, 0xEA000, 0xE9000, 0, 0, 0, 0, 0, 0, 0, 0);
}
