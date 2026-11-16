/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x006DFF78-0x006DFFC0 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "sys.h"
#include "battle/n28.h"

extern s8 dbgBtlTraceLightNo;


/* (&fxHitLightColors[i])->r: fxHitLightColors[i].r schedules differently. */
void DbgBtlTraceLightPage(void) {
    s32 n;
    s8 *p;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 3);
        switch (sysState.dbgCursor) {
        case 0:
            p = &dbgBtlTraceLightNo;
            if (dbgBtlTraceLightNo + DbgEditValue(0, 0, 1, 1.0f) < 0.0f) {
                n = 9;
            } else if (9.0f < dbgBtlTraceLightNo + DbgEditValue(0, 0, 1, 1.0f)) {
                n = 0;
            } else {
                n = dbgBtlTraceLightNo + DbgEditValue(0, 0, 1, 1.0f);
            }
            *p = n;
            break;
        case 1:
            (&fxHitLightColors[dbgBtlTraceLightNo])->r += DbgEditValue(2, 0, 3, 1.0f);
            break;
        case 2:
            (&fxHitLightColors[dbgBtlTraceLightNo])->g += DbgEditValue(2, 0, 3, 1.0f);
            break;
        case 3:
            (&fxHitLightColors[dbgBtlTraceLightNo])->b += DbgEditValue(2, 0, 3, 1.0f);
            break;
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, "%sTRACE%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "No. %d", dbgBtlTraceLightNo);
    DbgPrintf(2, 4, "LGT R %4d", (s32)(&fxHitLightColors[dbgBtlTraceLightNo])->r);
    DbgPrintf(6, 5, "G %4d", (s32)(&fxHitLightColors[dbgBtlTraceLightNo])->g);
    DbgPrintf(6, 6, "B %4d", (s32)(&fxHitLightColors[dbgBtlTraceLightNo])->b);
}
