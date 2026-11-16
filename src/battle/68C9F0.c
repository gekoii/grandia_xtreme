#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"

/* FxTraceUnit.ofs[j] (a when k = 0, b when k = 1) */
typedef struct {
    u8 pad[0x80];
    f32 x, y, z;
} VAt;

void FxTraceSetOffset(s32 i, s32 j, s32 k, f32 x, f32 y, f32 z) {
    f32 s = btlUnits[i].def->modelPack->unk5C;
    FxTraceUnit *o = &fxTraceUnits[i];
    VAt *v = (VAt *)((u8 *)o + (k * 16 + j * 32));
    v->x = x * s;
    v->y = y * s;
    v->z = z * s;
}
