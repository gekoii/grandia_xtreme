#include "common.h"
#include "gph.h"
#include "chr.h"
#include "battle/misc.h"
#include "battle/n31.h"

void MgcModelResetPartDisp(GphModel *dst, ChrModelData *src) {
    s32 n;
    GphPart *p;
    GphPart *q;

    p = dst->parts;
    n = dst->partCount;

    if (mgcWork.unk13 & 6) {
        for (; n > 0; n--) {
            p->flags &= ~8;
            p++;
        }
    } else {
        q = src->parts;
        for (; n > 0; n--) {
            p->flags |= q->flags & 8;
            q++;
            p++;
        }
    }
}
