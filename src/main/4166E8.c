#include "common.h"
#include "protos/main.h"
#include "party.h"




void PtyCharLevelDown(PtyUnit *obj) {
    s32 idx;
    s32 n;
    s32 a;
    PtyGrowth *rec;

    idx = obj->id - 0x1001;
    if (idx < 0) {
        return;
    }
    n = obj->level;
    if (n < 2) {
        return;
    }
    rec = &ptyGrowthTable[idx];
    a = rec->exp[n - 2];
    obj->exp = (f32)a + (f32)(rec->exp[n - 1] - a) * ptyLevelDownExpRate;
    PtyCharUpdateLevel(obj);
    PtyCharLevelDownStats(idx);
    PtyUnitCalcParams(obj, NULL, NULL);
}
