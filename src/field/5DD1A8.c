#include "common.h"
#include "protos/field.h"

s32 DbgFldTenantSearchDraw(s32);

void DbgFldTenantSearchDrawAll(void) {
    s32 i;

    for (i = 0; i < ChrTenantGetCount(); i++) {
        DbgFldTenantSearchDraw(ChrTenantAt(i));
    }
}
