#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/state.h"

extern f32 dbgFldFadeRed;
extern f32 dbgFldFadeGreen;
extern f32 dbgFldFadeBlue;
extern f32 dbgFldFadeAlpha;
extern f32 dbgFldFadeTime;
extern s32 dbgFldFadeLine;
extern s32 dbgFldFadeEditLine;
extern char dbgFldFadeStrFade[];
extern char dbgFldFadeStrR[];
extern char dbgFldFadeStrG[];
extern char dbgFldFadeStrB[];
extern char dbgFldFadeStrA[];
extern char dbgFldFadeStrTime[];


void DbgFldFadePage(void) {
    Pad *pad;

    pad = &padPort1;
    if (!(padPort1.held & 2)) {
        dbgFldFadeLine = DbgMenuLineSelect(dbgFldFadeLine, 3);
        switch (dbgFldFadeLine) {
        case 0:
            dbgFldFadeEditLine = dbgFldFadeLine;
            if (pad->repeatFast & 0x8000) {
                dbgFldFadeRed -= 1.0f;
            }
            if (pad->repeatFast & 0x2000) {
                dbgFldFadeRed += 1.0f;
            }
            break;
        case 1:
            dbgFldFadeEditLine = dbgFldFadeLine;
            if (pad->repeatFast & 0x8000) {
                dbgFldFadeGreen -= 1.0f;
            }
            if (pad->repeatFast & 0x2000) {
                dbgFldFadeGreen += 1.0f;
            }
            break;
        case 2:
            dbgFldFadeEditLine = dbgFldFadeLine;
            if (pad->repeatFast & 0x8000) {
                dbgFldFadeBlue -= 1.0f;
            }
            if (pad->repeatFast & 0x2000) {
                dbgFldFadeBlue += 1.0f;
            }
            break;
        case 3:
            dbgFldFadeEditLine = dbgFldFadeLine;
            if (pad->repeatFast & 0x8000) {
                dbgFldFadeAlpha -= 1.0f;
            }
            if (pad->repeatFast & 0x2000) {
                dbgFldFadeAlpha += 1.0f;
            }
            break;
        case 4:
            dbgFldFadeEditLine = dbgFldFadeLine;
            if (pad->repeatFast & 0x8000) {
                dbgFldFadeTime -= 1.0f;
            }
            if (pad->repeatFast & 0x2000) {
                dbgFldFadeTime += 1.0f;
            }
            break;
        }
        if (pad->repeatFast & 0x20) {
            GphFadeStart(&fldFades[0], 0, dbgFldFadeRed, dbgFldFadeGreen, dbgFldFadeBlue, dbgFldFadeAlpha, dbgFldFadeTime);
            GphFadeStart(&fldFades[1], 0, dbgFldFadeRed, dbgFldFadeGreen, dbgFldFadeBlue, dbgFldFadeAlpha, dbgFldFadeTime);
        }
    }
    DbgPrintf(2, 3, dbgFldFadeStrFade);
    DbgPrintf(3, 5, dbgFldFadeStrR, fldFades[0].color[0], fldFades[1].color[0]);
    DbgPrintf(3, 6, dbgFldFadeStrG, fldFades[0].color[1], fldFades[1].color[1]);
    DbgPrintf(3, 7, dbgFldFadeStrB, fldFades[0].color[2], fldFades[1].color[2]);
    DbgPrintf(3, 8, dbgFldFadeStrA, fldFades[0].color[3], fldFades[1].color[3]);
    DbgPrintf(3, 9, dbgFldFadeStrTime, dbgFldFadeTime);
}
