#include "common.h"
#include "protos/field.h"
#include "field/n14.h"


s32 ChrTenantOpWalkToPoint(void *arg0, ChrObj *a, s32 n, s32 p2) {
    u8 buf[16];

    if (a->opState != 3) {
        a->opState = 3;
        switch (n) {
        case 7:
            n = VuRandInt(a->areas[a->area]->pointCount) + 1;
            break;
        case 8:
            n = ChrTenantFindAreaPoint(a, buf, 0);
            break;
        case 9:
            /* s32 cast needed: with the header prototype this function is one insn shorter (38+ words off) */
            n = ((s32 (*)(ChrObj *, void *, s32))ChrTenantFindAreaPoint)(a, buf, 1);
            break;
        }
        ChrTenantAreaGetPoint(a, n, a->moveTarget);
    }
    a->headingDeg = a->heading * 0.0054931640625f;
    return ChrTenantMoveToward(a, a->moveTarget, p2, 1);
}
