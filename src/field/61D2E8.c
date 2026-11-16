#include "common.h"
#include "protos/field.h"
#include "field/n23.h"

void MapBorderSweep(s32 *);
void FldCollTrisSweep(s32 *);
void FldCollShapesSweep(s32 *);

s32 FldCollSweep(s32 *arg0) {
    s32 flags;
    s32 ok;

    flags = MapCollGetFlags();
    fldCollMeshHitCount = 0;
    fldCollTriTestTotal = 0;
    if (flags & 2) {
        MapBorderSweep(arg0);
    }
    ok = 0;
    if (*arg0) {
        ok = 1;
    }
    if (flags & 8) {
        FldCollTrisSweep(arg0);
    }
    if (*arg0) {
        ok = 1;
    }
    if (flags & 4) {
        FldCollShapesSweep(arg0);
    }
    if (*arg0) {
        ok = 1;
    }
    if (ok) {
        *arg0 = 1;
    } else {
        *arg0 = 0;
    }
    return 0;
}
