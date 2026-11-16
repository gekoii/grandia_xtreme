/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/menu.h"
#include "party.h"

void PtyEquipPreviewStats(s16, s16, u16 *);

void PtyStatusPreviewBuild(s16 arg0, s16 arg1) {
    u16 buf[24];
    s32 i;
    s32 kind;
    s32 lo;
    s32 hi;
    f32 f;

    PtyEquipPreviewStats(arg0, arg1, buf);
    for (i = 0; i < 6; i++) {
        itemWork->unk6D3C[i] = buf[i];
        itemWork->unk6D48[i] = buf[i + 6];
    }
    for (i = 0; i < 9; i++) {
        f = ptyCharCalcUnits->regMgcAttr[i];
        if (f < -100.0f) {
            kind = 0;
            lo = 0;
            hi = 100;
        } else if (f < 0.0f) {
            kind = 0;
            lo = f + 100.0;
            hi = 100;
        } else if (f == 0.0f) {
            kind = 1;
            lo = 100;
            hi = 100;
        } else if (f < 100.0f) {
            kind = 2;
            lo = 100;
            hi = f + 100.0;
        } else if (f < 150.0f) {
            kind = 3;
            lo = 100;
            hi = 200;
        } else if (f < 250.0f) {
            kind = 4;
            lo = 100;
            hi = f + 50.0f;
        } else {
            kind = 4;
            lo = 100;
            hi = 300;
        }
        itemWork->gauges[i].idx = i;
        itemWork->gauges[i].kind = kind;
        itemWork->gauges[i].lo = lo;
        itemWork->gauges[i].val = f;
        itemWork->gauges[i].hi = hi;
        itemWork->gauges2[i].id = i;
        itemWork->gauges2[i].val = ptyCharCalcUnits->regSta[i];
    }
}
