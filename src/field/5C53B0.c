#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

ChrTenantArea *ChrTenantAreaPickRandom(void) {
    u16 list[64];
    s32 i;
    u16 n;
    ChrTenantArea *e;

    n = 0;
    for (i = 0; i < chrTenantAreaTable->count; i++) {
        e = ChrTenantAreaGet(i);
        if ((u32)(e->kind - 1) < 2) {
            list[n] = i;
            n++;
            if (n >= 64) {
                break;
            }
        }
    }
    if (n == 0) {
        return NULL;
    }
    e = ChrTenantAreaGet(list[VuRandInt(n)]);
    e->kind += 10;
    return e;
}
