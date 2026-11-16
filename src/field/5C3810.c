#include "common.h"
#include "field/n14.h"

s32 ChrTenantOpSightOff(void *arg0, ChrObj *a) {
    ChrEnemyWork *s;

    s = a->sense;
    if (s == NULL) {
        return 0;
    }
    s->flags &= ~4;
    return 0;
}
