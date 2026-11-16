#include "common.h"
#include "field/n14.h"

s32 ChrTenantOpWait(void *arg0, ChrObj *a, s32 flag, s32 time) {
    a->headingDeg = a->heading * 0.0054931640625f;
    if (a->opTimer > 0) {
        a->opTimer--;
    } else {
        if (a->opState == 2) {
            a->opState = 0;
            if (a->sense != NULL) {
                CHR_SENSE(a)->flags &= ~8;
            }
            return 0;
        }
        a->opState = 2;
        a->opTimer = time;
        if (flag == 1) {
            if (a->sense != NULL) {
                CHR_SENSE(a)->flags |= 8;
            }
        }
        a->speed = 0.0f;
        a->vel[0] = 0.0f;
        a->vel[2] = 0.0f;
        if (a->htaArea == 0) {
            a->vel[1] = 0.0f;
        }
    }
    return 1;
}
