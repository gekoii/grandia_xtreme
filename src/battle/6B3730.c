#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/n31.h"


void BtlChrPartsSetGaObjFlag2(GphPart *p, s32 a) {
    s32 n = p->groupPartCount;

    for (; n > 0; n--) {
        ChrPartSetGaObjFlag2(p, a);
        p++;
    }
}
