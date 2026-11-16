#include "common.h"
#include "pad.h"
#include "gph.h"
#include "main/n02.h"
#include "protos/main.h"

extern u32 dbgPtLightLine;
extern s32 dbgPtLightCursor;
extern s32 dbgPtLightNo;
extern u8 dbgPtLightStrPointLight[];
extern u8 dbgPtLightStrLightNo[];
extern u8 dbgPtLightStrPosX[];
extern u8 dbgPtLightStrY[];
extern u8 dbgPtLightStrZ[];
extern u8 dbgPtLightStrRange[];
extern u8 dbgPtLightStrColorR[];
extern u8 dbgPtLightStrG[];
extern u8 dbgPtLightStrB[];
extern u8 dbgPtLightStrA[];
extern u8 dbgPtLightStrPower[];

void DbgPointLightPage(void) {
    GphLight *o;

    dbgPad = (Pad *)&padPort1;
    DbgMenuCursorDraw(2, dbgPtLightCursor + 5);
    o = (GphLight *)GphLightGet(dbgPtLightNo);
    if (!(dbgPad->held & 2)) {
        dbgPtLightLine = DbgMenuLineSelect(dbgPtLightLine, 10);
        switch (dbgPtLightLine) {
        case 0:
            dbgPtLightCursor = dbgPtLightLine;
            if ((dbgPad->repeatFast & 0x8000) && dbgPtLightNo != 0) {
                dbgPtLightNo--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgPtLightNo++;
            }
            break;
        case 1:
            dbgPtLightCursor = dbgPtLightLine;
            if (o != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    o->enable = 0;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    o->enable = 1;
                }
            }
            break;
        case 2:
        case 3:
        case 4:
            dbgPtLightCursor = dbgPtLightLine;
            if (o != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    o->dir[dbgPtLightLine - 2] -= 10.0f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    o->dir[dbgPtLightLine - 2] += 10.0f;
                }
            }
            break;
        case 5:
            dbgPtLightCursor = dbgPtLightLine;
            if (o != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    o->range -= 10.0f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    o->range += 10.0f;
                }
            }
            break;
        case 10:
            dbgPtLightCursor = dbgPtLightLine;
            if (o != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    o->intensity -= 10.0f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    o->intensity += 10.0f;
                }
            }
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            dbgPtLightCursor = dbgPtLightLine;
            if (o != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    o->color[dbgPtLightLine - 6] -= 1.0f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    o->color[dbgPtLightLine - 6] += 1.0f;
                }
            }
            break;
        }
    }
    DbgPrintf(2, 3, dbgPtLightStrPointLight);
    DbgPrintf(3, 5, dbgPtLightStrLightNo, dbgPtLightNo);
    if (o != NULL && o->type == 1) {
        if (o->enable == 1) {
            DbgPrintf(3, 6, dbgStrDispOn);
        } else {
            DbgPrintf(3, 6, dbgStrDispOff);
        }
        DbgPrintf(3, 7, dbgPtLightStrPosX, o->dir[0]);
        DbgPrintf(3, 8, dbgPtLightStrY, o->dir[1]);
        DbgPrintf(3, 9, dbgPtLightStrZ, o->dir[2]);
        DbgPrintf(3, 10, dbgPtLightStrRange, o->range);
        DbgPrintf(3, 11, dbgPtLightStrColorR, o->color[0]);
        DbgPrintf(3, 12, dbgPtLightStrG, o->color[1]);
        DbgPrintf(3, 13, dbgPtLightStrB, o->color[2]);
        DbgPrintf(3, 14, dbgPtLightStrA, o->color[3]);
        DbgPrintf(3, 15, dbgPtLightStrPower, o->intensity);
    }
}
