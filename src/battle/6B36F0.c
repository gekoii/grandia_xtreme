#include "common.h"
#include "gph.h"
#include "battle/n31.h"

void BtlChrPartsSetUvScroll(GphPart *p, s16 a, s16 b) {
    s32 n = p->groupPartCount;

    for (; n > 0; n--) {
        p->uvScroll[1] = a;
        p++;
    }
    p->uvScroll[3] = b;
}
