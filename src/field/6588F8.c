#include "common.h"
#include "grxsys.h"
#include "protos/field.h"



void FldSpriteSelectAnim(s32 arg0) {
    switch (arg0) {
    case 0:
        GphAnimSetSelect(sysGrxsysPac->animSets[0]);
        break;
    case 1:
        GphAnimSetSelect(sysGrxsysPac->animSets[1]);
        break;
    case 2:
        GphAnimSetSelect(sysGrxsysPac->animSets[2]);
        break;
    case 3:
        GphAnimSetSelect(sysGrxsysPac->animSets[3]);
        break;
    case 4:
        GphAnimSetSelect(sysGrxsysPac->animSets[4]);
        break;
    case 5:
        GphAnimSetSelect(sysGrxsysPac->animSets[5]);
        break;
    case 6:
        GphAnimSetSelect(sysGrxsysPac->animSet34);
        break;
    case 7:
        GphAnimSetSelect(sysGrxsysPac->animSets[4]);
        break;
    case 8:
        GphAnimSetSelect(sysGrxsysPac->animSets[4]);
        break;
    }
}
