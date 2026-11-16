#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

s32 FldMemAlloc(s32);

ChrObj *ChrObjArrayAlloc(s32 n) {
    ChrObj *objs;
    ChrMotTable *pools;
    s32 i;

    objs = (ChrObj *)FldMemAlloc(n * sizeof(ChrObj));
    pools = (ChrMotTable *)FldMemAlloc(n << 8);
    for (i = 0; i < n; i++) {
        objs[i].motTable = pools;
        ChrMotTableClear(pools);
        pools++;
    }
    return objs;
}
