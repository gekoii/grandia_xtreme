#include "common.h"
#include "gph.h"
#include "battle/n31.h"

void VuVecCopy(void *, s32);

void BtlChrPartsSetColor(GphPart *p, s32 a) {
    s32 n = p->groupPartCount;

    for (; n > 0; n--) {
        VuVecCopy(p->color, a);
        p++;
    }
}
