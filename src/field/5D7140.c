/* rodata: 0x006718B8-0x00671968 */
#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldLinesLine;

void DbgFldLinesPage(void) {
    Pad *pad;
    u16 btn;

    pad = &padPort1;
    DbgDrawCursor(3, dbgFldLinesLine + 5);
    if (!(pad->held & 2)) {
        btn = pad->repeatFast;
        if (btn & 0x1000) {
            if (dbgFldLinesLine != 0) {
                dbgFldLinesLine--;
            } else {
                dbgFldLinesLine = 10;
            }
        }
        if (btn & 0x4000) {
            if (dbgFldLinesLine < 10) {
                dbgFldLinesLine++;
            } else {
                dbgFldLinesLine = 0;
            }
        }
        if (btn & 0x8000) {
            if ((dbgFldDispLines >> dbgFldLinesLine) & 1) {
                dbgFldDispLines ^= 1 << dbgFldLinesLine;
            }
        }
        if (btn & 0x2000) {
            dbgFldDispLines |= 1 << dbgFldLinesLine;
        }
    }
    DbgPrintf(4, 4, "DISP LINES");
    DbgPrintf(4, 5, "HTA          %s", dbgFldOffOnStr[dbgFldDispLines & 1]);
    DbgPrintf(4, 6, "TENANT LIMIT %s", dbgFldOffOnStr[(dbgFldDispLines >> 1) & 1]);
    DbgPrintf(4, 7, "BORDER CHR   %s", dbgFldOffOnStr[(dbgFldDispLines >> 2) & 1]);
    DbgPrintf(4, 8, "POLYGON      %s", dbgFldOffOnStr[(dbgFldDispLines >> 3) & 1]);
    DbgPrintf(4, 9, "BORDER CAM   %s", dbgFldOffOnStr[(dbgFldDispLines >> 4) & 1]);
    DbgPrintf(4, 10, "TENANT LIMIT %s", dbgFldOffOnStr[(dbgFldDispLines >> 5) & 1]);
    DbgPrintf(4, 11, "POINT LIGHT  %s", dbgFldOffOnStr[(dbgFldDispLines >> 6) & 1]);
    DbgPrintf(4, 12, "TENANT SERCH %s", dbgFldOffOnStr[(dbgFldDispLines >> 7) & 1]);
    DbgPrintf(4, 13, "CROSS LINE   %s", dbgFldOffOnStr[(dbgFldDispLines >> 8) & 1]);
    DbgPrintf(4, 14, "CHR COL F    %s", dbgFldOffOnStr[(dbgFldDispLines >> 9) & 1]);
    DbgPrintf(4, 15, "CHR COL B    %s", dbgFldOffOnStr[(dbgFldDispLines >> 10) & 1]);
}
