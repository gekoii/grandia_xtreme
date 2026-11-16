/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

extern s32 dbgPtclGenIndex;

void DbgPtclGenPage(void) {
    s32 v;
    s32 w;
    s32 *p;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        if (padTrigger & 4) {
            if (--sysState.dbgPage < 0) {
                sysState.dbgPage = 4;
            }
            sysState.dbgCursor = 0;
        } else if (padTrigger & 8) {
            if (++sysState.dbgPage >= 5) {
                sysState.dbgPage = 0;
            }
            sysState.dbgCursor = 0;
        }
        if (sysState.dbgCursor == 0) {
            dbgPtclGenIndex += DbgEditValue(0, 0, 3, 1.0f);
            v = dbgPtclGenIndex;
            p = &dbgPtclGenIndex;
            if (v < 0) {
                w = ptclMainPool->genCount - 1;
            } else {
                w = (ptclMainPool->genCount - 1 < v) ? 0 : v;
            }
            *p = w;
        }
    }
    switch (sysState.dbgPage) {
    case 0:
        DbgPtclGenPosPage(dbgPtclGenIndex);
        break;
    case 1:
        DbgPtclGenEmitPage(dbgPtclGenIndex);
        break;
    case 2:
        DbgPtclGenScalePage(dbgPtclGenIndex);
        break;
    case 3:
        DbgPtclGenColorPage(dbgPtclGenIndex);
        break;
    case 4:
        DbgPtclGenRotPage(dbgPtclGenIndex);
        break;
    }
    DbgPrintf(3, 2, dbgPtclGenTitleFmt, dbgPtclGenIndex, dbgPtclTitleColorStr, sysState.dbgPage + 1, dbgPtclColor0Str);
}
