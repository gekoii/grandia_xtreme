#include "common.h"
#include "gph.h"
#include "battle/n31.h"

void BtlChrPartsSetHidden(GphPart *p, s8 mode) {
    s32 n = p->groupPartCount;

    switch (mode) {
    case 0:
        for (; n > 0; n--) {
            p->flags |= 8;
            p++;
        }
        break;
    case 1:
        for (; n > 0; n--) {
            p->flags &= ~8;
            p++;
        }
        break;
    default:
        for (; n > 0; n--) {
            p->flags ^= 8;
            p++;
        }
        break;
    }
}
