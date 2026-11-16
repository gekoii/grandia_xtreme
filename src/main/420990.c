/* rodata: 0x004A3730-0x004A3790 */
/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "sys.h"
#include "protos/main.h"


extern s32 dbgLoadFrames;
extern f32 dbgLoadMaxPrev;
extern f32 dbgLoadMinPrev;
extern f32 dbgLoadMax;
extern f32 dbgLoadMin;

void DbgLoadMeterDraw(s32 arg0) {
    f32 f;

    if (arg0 != 0) {
        return;
    }
    if (sysState.debugMode == 0) {
        return;
    }
    sysState.frameTime /= sysState.frameMode + 1;
    sysState.frameTimeDelta /= sysState.frameMode + 1;
    if (sysState.frameTime > 60.0f) {
        if (sysState.frameTime > 80.0f) {
            if (sysState.frameTime > 100.0f) {
                DbgTextSetColor(2, 0x80808000);
            } else {
                DbgTextSetColor(2, 0x80505080);
            }
        } else {
            DbgTextSetColor(2, 0x6040A0C0);
        }
    } else {
        DbgTextSetColor(2, 0x60808080);
    }
    if (sysState.frameTimeDelta > 60.0f) {
        if (sysState.frameTimeDelta > 80.0f) {
            if (sysState.frameTimeDelta > 100.0f) {
                DbgTextSetColor(3, 0x80808000);
            } else {
                DbgTextSetColor(3, 0x80505080);
            }
        } else {
            DbgTextSetColor(3, 0x6040A0C0);
        }
    } else {
        DbgTextSetColor(3, 0x60808080);
    }
    DbgPrintf(0x12, 1, "\\c7 Ver:(%s) C:\\c2 %3.1hf%%, \\c7 D:\\c3 %3.1hf%%\\c7 ", "Aug  2 2002", sysState.frameTime, sysState.frameTimeDelta);
    if (dbgLoadFrames == 180) {
        dbgLoadMaxPrev = dbgLoadMax;
        dbgLoadMinPrev = dbgLoadMin;
        dbgLoadMin = 99999.0f;
        dbgLoadFrames = 0;
        dbgLoadMax = 0.0f;
    } else {
        dbgLoadFrames++;
        f = sysState.frameTime;
        dbgLoadMax = (f < dbgLoadMax) ? dbgLoadMax : f;
        dbgLoadMin = (dbgLoadMin < f) ? dbgLoadMin : f;
    }
    if (sysState.pauseMode != 0 && (sysState.loopFrameCount & 0x1F) < 0x10) {
        DbgPrintf(0x2B, 2, "\\c6 <PAUSE> \\c7 ");
    }
}
