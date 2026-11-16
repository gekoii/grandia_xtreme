/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

extern void DbgPtclCtrlGenPosPage(PtclCtrl *, PtclGenDef *);
extern void DbgPtclCtrlGenEmitPage(PtclCtrl *, PtclGenDef *);
extern void DbgPtclCtrlGenScalePage(PtclCtrl *, PtclGenDef *);
extern void DbgPtclCtrlGenColorPage(PtclCtrl *, PtclGenDef *);
extern void DbgPtclCtrlGenRotPage(PtclCtrl *, PtclGenDef *);
extern s32 dbgPtclCtrlGenIndex;

void DbgPtclCtrlGenPage(PtclCtrl *arg0) {
    PtclGenDef *s;
    s32 v;

    s = &arg0->keys[dbgPtclCtrlGenIndex];
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
            v = dbgPtclCtrlGenIndex += DbgEditValue(0, 0, 3, 1.0f);
            dbgPtclCtrlGenIndex = (v < 0) ? 3 : ((v > 3) ? 0 : v);
            s = &arg0->keys[dbgPtclCtrlGenIndex];
        }
    }
    switch (sysState.dbgPage) {
    case 0:
        DbgPtclCtrlGenPosPage(arg0, s);
        break;
    case 1:
        DbgPtclCtrlGenEmitPage(arg0, s);
        break;
    case 2:
        DbgPtclCtrlGenScalePage(arg0, s);
        break;
    case 3:
        DbgPtclCtrlGenColorPage(arg0, s);
        break;
    case 4:
        DbgPtclCtrlGenRotPage(arg0, s);
        break;
    }
    DbgPrintf(3, 2, dbgPtclGenTitleFmt, dbgPtclCtrlGenIndex, dbgPtclTitleColorStr, sysState.dbgPage + 1, dbgPtclColor0Str);
}
