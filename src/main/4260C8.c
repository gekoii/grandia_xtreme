#include "common.h"
#include "protos/main.h"
#include "gph.h"

extern s32 dbgSndEqChannel;
extern s32 dbgSndEqTimer;
extern u8 dbgSndEqStrEqualizer[];
extern u8 dbgSndEqStrLevel[];
extern u8 dbgSndEqStrErr[];
extern s32 dbgSndEqLevels[];

void DbgSoundEqPage(void) {
    GphBatchQuad prims[30];
    GphBatchQuad *p;
    s32 ret;
    s32 i;
    s32 y;

    if (dbgSndEqTimer > 0) {
        ret = SndGetEqLevels(dbgSndEqChannel, dbgSndEqLevels);
        dbgSndEqTimer = 0;
    } else {
        ret = 0;
        dbgSndEqTimer++;
    }
    SndPcmMonitorOn();
    DbgPrintf(5, 5, dbgSndEqStrEqualizer, ret);
    if (ret >= 0) {
        p = prims;
        y = 0x78;
        for (i = 0; i < 7; i++) {
            DbgPrintf(5, i + 5, dbgSndEqStrLevel, dbgSndEqLevels[i]);
            p->type = 5;
            p->v[0].r = 0xB4; p->v[0].g = 0x40; p->v[0].b = 0; p->v[0].a = 0x40;
            p->v[1].r = 0xB4; p->v[1].g = 0x40; p->v[1].b = 0; p->v[1].a = 0x40;
            p->v[2].r = 0xB4; p->v[2].g = 0x40; p->v[2].b = 0; p->v[2].a = 0x40;
            p->v[3].r = 0xB4; p->v[3].g = 0x40; p->v[3].b = 0; p->v[3].a = 0x40;
            p->v[0].x = 0xF0;
            p->v[0].y = y;
            p->v[1].x = 0xF0;
            p->v[1].y = y + 8;
            p->v[2].x = dbgSndEqLevels[i] + 0xF0;
            p->v[2].y = y;
            p->v[3].x = dbgSndEqLevels[i] + 0xF0;
            p->v[3].y = y + 8;
            p->z = 0;
            y += 10;
            p++;
            p->type = 5;
            p->v[0].r = 0xB4; p->v[0].g = 0x40; p->v[0].b = 0; p->v[0].a = 0x40;
            p->v[1].r = 0xB4; p->v[1].g = 0x40; p->v[1].b = 0; p->v[1].a = 0x40;
            p->v[2].r = 0xB4; p->v[2].g = 0x40; p->v[2].b = 0; p->v[2].a = 0x40;
            p->v[3].r = 0xB4; p->v[3].g = 0x40; p->v[3].b = 0; p->v[3].a = 0x40;
            p->v[0].x = 0xF0;
            p->v[0].y = y;
            p->v[1].x = 0xF0;
            p->v[1].y = y + 8;
            p->v[2].x = (dbgSndEqLevels[i] + dbgSndEqLevels[i + 1]) / 2 + 0xF0;
            p->v[2].y = y;
            p->v[3].x = (dbgSndEqLevels[i] + dbgSndEqLevels[i + 1]) / 2 + 0xF0;
            p->v[3].y = y + 8;
            p->z = 0;
            y += 10;
            p++;
            p->type = 5;
            p->v[0].r = 0xB4; p->v[0].g = 0x40; p->v[0].b = 0; p->v[0].a = 0x40;
            p->v[1].r = 0xB4; p->v[1].g = 0x40; p->v[1].b = 0; p->v[1].a = 0x40;
            p->v[2].r = 0xB4; p->v[2].g = 0x40; p->v[2].b = 0; p->v[2].a = 0x40;
            p->v[3].r = 0xB4; p->v[3].g = 0x40; p->v[3].b = 0; p->v[3].a = 0x40;
            p->v[0].x = 0xF0;
            p->v[0].y = y;
            p->v[1].x = 0xF0;
            p->v[1].y = y + 8;
            p->v[2].x = (dbgSndEqLevels[i] + dbgSndEqLevels[i + 1]) / 2 + VuRandInt(4) + 0xF0;
            p->v[2].y = y;
            p->v[3].x = (dbgSndEqLevels[i] + dbgSndEqLevels[i + 1]) / 2 + VuRandInt(4) + 0xF0;
            p->v[3].y = y + 8;
            p->z = 0;
            y += 10;
            p++;
        }
        p->type = 5;
        p->v[0].r = 0xB4; p->v[0].g = 0x40; p->v[0].b = 0; p->v[0].a = 0x40;
        p->v[1].r = 0xB4; p->v[1].g = 0x40; p->v[1].b = 0; p->v[1].a = 0x40;
        p->v[2].r = 0xB4; p->v[2].g = 0x40; p->v[2].b = 0; p->v[2].a = 0x40;
        p->v[3].r = 0xB4; p->v[3].g = 0x40; p->v[3].b = 0; p->v[3].a = 0x40;
        p->v[0].x = 0xF0;
        p->v[0].y = y;
        p->v[1].x = 0xF0;
        p->v[1].y = y + 8;
        p->v[2].x = dbgSndEqLevels[7] + 0xF0;
        p->v[2].y = y;
        p->v[3].x = dbgSndEqLevels[7] + 0xF0;
        p->v[3].y = y + 8;
        p->z = 0;
        p++;
        y += 10;
        p->type = 5;
        p->v[0].r = 0xB4; p->v[0].g = 0x40; p->v[0].b = 0; p->v[0].a = 0x40;
        p->v[1].r = 0xB4; p->v[1].g = 0x40; p->v[1].b = 0; p->v[1].a = 0x40;
        p->v[2].r = 0xB4; p->v[2].g = 0x40; p->v[2].b = 0; p->v[2].a = 0x40;
        p->v[3].r = 0xB4; p->v[3].g = 0x40; p->v[3].b = 0; p->v[3].a = 0x40;
        p->v[0].x = 0xF0;
        p->v[0].y = y;
        p->v[1].x = 0xF0;
        p->v[1].y = y + 8;
        p->v[2].x = (dbgSndEqLevels[7] + dbgSndEqLevels[i - 1]) / 2 + 0xF0;
        p->v[2].y = y;
        p->v[3].x = (dbgSndEqLevels[7] + dbgSndEqLevels[i - 1]) / 2 + 0xF0;
        p->v[3].y = y + 8;
        p->z = 0;
        p++;
        y += 10;
        p->type = 5;
        p->v[0].r = 0xB4; p->v[0].g = 0x40; p->v[0].b = 0; p->v[0].a = 0x40;
        p->v[1].r = 0xB4; p->v[1].g = 0x40; p->v[1].b = 0; p->v[1].a = 0x40;
        p->v[2].r = 0xB4; p->v[2].g = 0x40; p->v[2].b = 0; p->v[2].a = 0x40;
        p->v[3].r = 0xB4; p->v[3].g = 0x40; p->v[3].b = 0; p->v[3].a = 0x40;
        p->v[0].x = 0xF0;
        p->v[0].y = y;
        p->v[1].x = 0xF0;
        p->v[1].y = y + 8;
        p->v[2].x = (dbgSndEqLevels[7] + dbgSndEqLevels[i - 1]) / 2 + VuRandInt(4) + 0xF0;
        p->v[2].y = y;
        p->v[3].x = (dbgSndEqLevels[7] + dbgSndEqLevels[i - 1]) / 2 + VuRandInt(4) + 0xF0;
        p->v[3].y = y + 8;
        p->z = 0;
        p++;
        p->type = 0;
        GphBatchDrawQuads(prims);
        return;
    }
    DbgPrintf(5, 6, dbgSndEqStrErr, ret);
    if (dbgSndEqChannel == 1) {
        dbgSndEqChannel = 2;
    } else {
        dbgSndEqChannel = 1;
    }
}
