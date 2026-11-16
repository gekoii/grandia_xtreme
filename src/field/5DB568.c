#include "common.h"
#include "pad.h"
#include "protos/field.h"


extern s32 dbgFldCamScriptLine;
extern s32 dbgFldCamScriptTop;
extern char dbgCamScrStrListMax[];
extern char dbgCamScrStrDataNo[];


void DbgFldCamScriptListPage(void) {
    Pad *pad;
    u16 btn;
    s32 cnt;
    s32 i;
    s32 j;

    pad = &padPort1;
    cnt = 0;
    while (CamScrGet(cnt) != NULL) {
        cnt++;
    }
    DbgPrintf(4, 5, dbgCamScrStrListMax, cnt);
    DbgDrawCursor(3, dbgFldCamScriptLine + 6);
    if (!(pad->held & 2)) {
        btn = pad->repeatFast;
        if (btn & 0x1000) {
            if (dbgFldCamScriptTop != 0) {
                if (dbgFldCamScriptLine >= 7) dbgFldCamScriptLine--;
                else dbgFldCamScriptTop--;
            } else if (dbgFldCamScriptLine != 0) {
                dbgFldCamScriptLine--;
            }
        }
        if (btn & 0x4000) {
            if (dbgFldCamScriptLine < 6) {
                dbgFldCamScriptLine++;
            } else if (cnt >= 14) {
                if (dbgFldCamScriptTop + 13 < cnt) dbgFldCamScriptTop++;
                else if (dbgFldCamScriptTop + dbgFldCamScriptLine < cnt - 1) dbgFldCamScriptLine++;
            } else if (dbgFldCamScriptTop < cnt) {
                dbgFldCamScriptTop++;
            }
        }
        if (pad->trigger & 0x20) {
            CamScrStart(dbgFldCamScriptTop + dbgFldCamScriptLine);
        }
    }
    for (i = 0, j = dbgFldCamScriptTop; i < 13 && j < cnt; i++, j++) {
        DbgPrintf(4, i + 6, dbgCamScrStrDataNo, j);
    }
}
