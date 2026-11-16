/* rodata: 0x00675828-0x00675878 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

typedef struct {
    s32 id;
    s32 z;
    u8 c0[4];
    s16 x0;
    s16 y0;
    u8 c1[4];
    s16 x1;
    s16 y1;
    u8 pad18[8];
    s32 x20;
    u8 pad24[0x100 - 0x24];
} Ent;


void MiniBallDebugDraw(void) {
    Ent e;
    s32 i;
    MiniBall *m;

    if (EvtGlobalFlagGet(0x5F8) == 0) {
        return;
    }
    for (i = 0; i < miniBallData->hdr->wallCount; i++) {
        e.id = 9;
        e.z = 1;
        e.x20 = 0;
        e.x0 = miniBallData->walls[i].x0;
        e.y0 = miniBallData->walls[i].y0;
        e.x1 = miniBallData->walls[i].x1;
        e.y1 = miniBallData->walls[i].y1;
        e.c0[0] = 0x80;
        e.c0[1] = 0xFF;
        e.c0[2] = 0x80;
        e.c0[3] = 0x80;
        e.c1[0] = 0x80;
        e.c1[1] = 0xFF;
        e.c1[2] = 0x80;
        e.c1[3] = 0x80;
        GphBatchDrawLines(&e);
    }
    if (EvtGlobalFlagGet(0x5F8) != 1) {
        return;
    }
    if (miniBallUi->input.trigger & 0x10) {
        miniBallWork->dbgBall++;
        if (miniBallWork->dbgBall == 20) {
            miniBallWork->dbgBall = -1;
        }
    }
    if (miniBallWork->dbgBall != -1) {
        m = &miniBallWork->game.balls[miniBallWork->dbgBall];
        DbgPrintf(0xF, 0xA, "num : %d", miniBallWork->dbgBall);
        DbgPrintf(0xF, 0xB, "pos : % 8.3f.% 8.3f", m->pos[0], m->pos[2]);
        DbgPrintf(0xF, 0xC, "vec : % 8.3f.% 8.3f", m->vel[0], m->vel[2]);
        DbgPrintf(0xF, 0xD, "spd : % 8.3f", m->speed);
    }
}
