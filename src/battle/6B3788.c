#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/n31.h"


void BtlChrPartsSetAttr(GphPart *p, s32 a, s32 b) {
    s32 n = p->groupPartCount;

    while (n > 0) {
        n--;
        ChrPartSetAttr(p, a, b);
        p++;
    }
}
