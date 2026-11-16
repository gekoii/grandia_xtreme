#include "common.h"
#include "protos/field.h"
#include "party.h"


void PtyCharCalcSp(s16 arg0, s32 *arg1, s32 *arg2) {
    PtyUnitInitFromChar(ptyCharCalcUnits, arg0 + 0x1001);
    PtyUnitCalcParams(ptyCharCalcUnits, 0, 0);
    *arg1 = ptyCharCalcUnits->sp;
    *arg2 = ptyCharCalcUnits->maxSp;
}
