#include "common.h"
#include "pad.h"
#include "protos/main.h"



extern f32 dbgScreenFadeRed;
extern f32 dbgScreenFadeGreen;
extern f32 dbgScreenFadeBlue;
extern f32 dbgScreenFadeAlpha;
extern f32 dbgScreenFadeTime;
extern u32 dbgFadeLine;
extern s32 dbgFadeCursor;
extern u8 dbgFadeStrFade[];
extern u8 dbgFadeStrR[];
extern u8 dbgFadeStrG[];
extern u8 dbgFadeStrB[];
extern u8 dbgFadeStrA[];
extern u8 dbgFadeStrTime[];

void DbgFadePage(void) {
    dbgPad = &padPort1;
    DbgMenuCursorDraw(2, dbgFadeCursor + 5);
    if (!(dbgPad->held & 2)) {
        dbgFadeLine = DbgMenuLineSelect(dbgFadeLine, 3);
        switch (dbgFadeLine) {
        case 0:
            dbgFadeCursor = dbgFadeLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgScreenFadeRed -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgScreenFadeRed += 1.0f;
            }
            break;
        case 1:
            dbgFadeCursor = dbgFadeLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgScreenFadeGreen -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgScreenFadeGreen += 1.0f;
            }
            break;
        case 2:
            dbgFadeCursor = dbgFadeLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgScreenFadeBlue -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgScreenFadeBlue += 1.0f;
            }
            break;
        case 3:
            dbgFadeCursor = dbgFadeLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgScreenFadeAlpha -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgScreenFadeAlpha += 1.0f;
            }
            break;
        case 4:
            dbgFadeCursor = dbgFadeLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgScreenFadeTime -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgScreenFadeTime += 1.0f;
            }
            break;
        }
        if (dbgPad->repeatFast & 0x20) {
            GphScreenFadeStart(0, dbgScreenFadeRed, dbgScreenFadeGreen, dbgScreenFadeBlue, dbgScreenFadeAlpha, dbgScreenFadeTime);
        }
    }
    DbgPrintf(2, 3, dbgFadeStrFade);
    DbgPrintf(3, 5, dbgFadeStrR, dbgScreenFadeRed);
    DbgPrintf(3, 6, dbgFadeStrG, dbgScreenFadeGreen);
    DbgPrintf(3, 7, dbgFadeStrB, dbgScreenFadeBlue);
    DbgPrintf(3, 8, dbgFadeStrA, dbgScreenFadeAlpha);
    DbgPrintf(3, 9, dbgFadeStrTime, dbgScreenFadeTime);
}
