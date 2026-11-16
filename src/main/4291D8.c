#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"



extern s32 dbgProgLine;
extern s32 dbgProgCursor;
extern char dbgProgStrProgramCallMode[];
extern char dbgProgStrProgram[];
extern char dbgProgStrProgramNotEntry[];

void DbgProgramCallPage(void) {
    s32 i;

    DbgPrintf(2, 3, dbgProgStrProgramCallMode);
    dbgPad = &padPort1;
    DbgMenuCursorDraw(2, dbgProgCursor + 5);
    if (!(dbgPad->held & 2)) {
        dbgProgLine = DbgMenuLineSelect(dbgProgLine, 6);
        if (dbgProgLine < 7) {
          if (dbgProgLine >= 0) {
            dbgProgCursor = dbgProgLine;
            if (FileGetReadState() == -1) {
                if (dbgPad->repeatFast & 0x8000) {
                    if (sysState.requested[dbgProgLine] != 0) {
                        sysState.requested[dbgProgLine]--;
                    } else {
                        sysState.requested[dbgProgLine] = 6;
                    }
                }
                if (dbgPad->repeatFast & 0x2000) {
                    sysState.requested[dbgProgLine]++;
                }
            }
            if (sysState.requested[dbgProgLine] >= 7) {
                sysState.requested[dbgProgLine] = 0;
            }
          }
        }
    }
    for (i = 0; i < 8; i++) {
        if (sysState.requested[i] != 0xFF) {
            DbgPrintf(3, i + 5, dbgProgStrProgram, i, sysProgramNames[sysState.requested[i]]);
        } else {
            DbgPrintf(3, i + 5, dbgProgStrProgramNotEntry, i);
        }
    }
}
