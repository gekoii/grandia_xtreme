#include "common.h"
#include "field/n14.h"

s32 ChrTenantOpSenseOff(void *arg0, ChrObj *a) {
    ChrEnemyWork *s;

    s = a->sense;
    if (s == NULL) {
        return 0;
    }
    s->flags &= ~5;
    return 0;
}
