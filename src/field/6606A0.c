#include "common.h"
#include "field/event.h"

void EvtTenantPointReqsReset(void) {
    s32 *p;
    s32 i;
    s32 a, b;

    /* word stores: tenant = 0xFFFF clears the padding after it too */
    p = (s32 *)evtTenantPointReqs;
    a = 0xFFFF;
    b = 0xFF;
    i = 0x3F;
    do {
        p[0] = a;
        i--;
        p[1] = b;
        p += 2;
    } while (i >= 0);
}
