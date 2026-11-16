/* rodata: 0x006E2C08-0x006E2C30 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/camera.h"
#include "pad.h"
#include "sys.h"
#include "battle/n34.h"

void VuSinCosAngle(f32 *, f32 *, s32);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void DbgBtlCharWorkPage(void) {
    BtlUnit *u;
    Pad *pad;
    s32 dx, dy;
    f32 d;
    f32 s, c;
    s32 v;
    s32 *idx;

    idx = &dbgBtlCharNo;
    u = &btlUnits[dbgBtlCharNo];
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        dx = 0x80 - pad->leftX;
        dy = pad->leftY - 0x80;
        d = dx * dx + dy * dy;
        if (d > 1024.0f) {
            VuSinCosAngle(&s, &c, BtlAtan2Angle(dx, dy) + btlCamYaw);
            d = fsqrt(d) - 32.0f;
            u->pos[0] += d * s * 0.125f;
            u->pos[2] += d * c * 0.125f;
        }
        DbgBtlPageSelect(pad, 3);
        if (sysState.dbgCursor == 0) {
            if ((dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f)) < 0.0f) v = 15;
            else if ((dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f)) > 15.0f) v = 0;
            else v = (dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f));
            *idx = v;
        }
    }
    switch (sysState.dbgPage) {
    case 0:
        DbgBtlCharWorkTransformPage(dbgBtlCharNo);
        break;
    case 1:
        DbgBtlCharWorkStatusPage(dbgBtlCharNo);
        break;
    case 2:
        DbgBtlCharWorkAttackPage(dbgBtlCharNo);
        break;
    case 3:
        DbgBtlCharWorkMotionPage(dbgBtlCharNo);
        break;
    }
    DbgPrintf(2, 2, "%sCHAR WORK%s page %d/4", dbgBtlColor1Str, dbgBtlColor0Str, sysState.dbgPage + 1);
    DbgPrintf(2, 3, "CHAR (%d)", dbgBtlCharNo);
}
