#include "common.h"
#include "battle/n34.h"

void DbgBtlHitMarkAdd(f32 x, f32 z, f32 radius) {
    DbgBtlHitMark *e = &dbgBtlHitMarks[dbgBtlHitMarkNext];
    if (++dbgBtlHitMarkNext >= 16) {
        dbgBtlHitMarkNext = 0;
    }
    e->alpha = 0x80;
    e->x = x;
    e->z = z;
    e->radius = radius;
}
