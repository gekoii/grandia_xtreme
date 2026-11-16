#include "common.h"
#include "pad.h"
#include "protos/field.h"


extern s32 dbgFldCounterEditColumn;
extern s32 dbgFldCounterEditLine;
extern u8 dbgCounterEditStrRow[];

void DbgFldCounterEditPage(u8 *vals, s32 n, u8 *marks) {
    Pad *pad = &padPort1;
    s32 idx;
    s32 i;
    s32 j;

    DbgDrawCursor(dbgFldCounterEditColumn * 11 + 2, dbgFldCounterEditLine + 6);
    if ((pad->repeatFast & 0x1000) && dbgFldCounterEditLine != 0) {
        dbgFldCounterEditLine--;
    }
    if ((pad->repeatFast & 0x4000) && dbgFldCounterEditLine < 11) {
        dbgFldCounterEditLine++;
    }
    if (pad->repeatFast & 0x8000) {
        if (dbgFldCounterEditColumn != 0) {
            dbgFldCounterEditColumn--;
        } else {
            dbgFldCounterEditColumn = 3;
        }
    }
    if (pad->repeatFast & 0x2000) {
        if (dbgFldCounterEditColumn < 3) {
            dbgFldCounterEditColumn++;
        } else {
            dbgFldCounterEditColumn = 0;
        }
    }
    idx = dbgFldCounterEditLine * 4 + dbgFldCounterEditColumn;
    if (pad->repeatFast & 0x20) {
        vals[idx]++;
    }
    if (pad->repeatFast & 0x80) {
        vals[idx]--;
    }
    for (i = 0, j = 0; i < n && j < n; j++) {
        u8 *v = &vals[i];
        u8 *m = &marks[i];
        DbgPrintf(3, j + 6, dbgCounterEditStrRow, m, v[0], m + 1, v[1], m + 2, v[2], m + 3, v[3]);
        i += 4;
        if (i >= n) break;
    }
}
