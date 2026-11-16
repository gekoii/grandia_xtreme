/* rodata: 0x00671968-0x00671990 */
#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/event.h"

extern s32 dbgFldScenarioListLine;
extern s32 dbgFldScenarioListTop;

void DbgFldScenarioListPage(void) {
    u16 buf[256];
    u16 *p;
    u16 id;
    s32 n;
    s32 i;
    s32 j;
    Pad *pad;

    pad = &padPort1;
    DbgPrintf(4, 5, "SCENARIO LIST");
    p = evtScnrWork->index;
    if (p == (u16 *)-1) {
        return;
    }
    n = 0;
    for (;;) {
        id = *p++;
        if (*p++ == 0xFFFF) {
            break;
        }
        buf[n++] = id;
    }
    DbgDrawCursor(3, dbgFldScenarioListLine + 6);
    if (!(pad->held & 2)) {
        if (pad->repeatFast & 0x1000) {
            if (dbgFldScenarioListTop != 0) {
                if (dbgFldScenarioListLine >= 7) {
                    dbgFldScenarioListLine--;
                } else {
                    dbgFldScenarioListTop--;
                }
            } else if (dbgFldScenarioListLine != 0) {
                dbgFldScenarioListLine--;
            }
        }
        if (pad->repeatFast & 0x4000) {
            if (dbgFldScenarioListLine < 6) {
                dbgFldScenarioListLine++;
            } else if (dbgFldScenarioListTop + 13 < n) {
                dbgFldScenarioListTop++;
            } else if (dbgFldScenarioListTop + dbgFldScenarioListLine < n - 1) {
                dbgFldScenarioListLine++;
            }
        }
        if (pad->trigger & 0x20) {
            EvtScnrCall(buf[dbgFldScenarioListTop + dbgFldScenarioListLine]);
        }
    }
    j = dbgFldScenarioListTop;
    i = 0;
    for (;;) {
        if (i >= 13) {
            break;
        }
        DbgPrintf(4, i + 6, "%03d : ID <%04x> ", j, buf[j]);
        j++;
        i++;
        if (j >= n) {
            break;
        }
    }
}
