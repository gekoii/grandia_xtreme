/* rodata: 0x00673578-0x00673588 */
#include "common.h"
#include "protos/field.h"
#include "field/n16.h"

void VuSinCosDeg(f32 *, f32 *, f32);
void VuVecCopy(VuVec4 *, VuVec4 *);

void DbgFldCursorDraw(void) {
    VuVec4 v[2];
    VuVec4 size;
    u32 cols[3] = { 0x800000FF, 0x80FF00FF, 0x8000FFFF };
    f32 s, c;
    f32 dx, dz;
    s32 i;

    size.x = 30.0f;
    size.y = 30.0f;
    size.z = 30.0f;
    for (i = 0; i < 2; i++) {
        VuVecCopy(&v[0], &dbgFldMoveCursorPos);
        VuVecCopy(&v[1], &dbgFldMoveCursorPos);
        v[0].w = 1.0f;
        v[1].w = 1.0f;
        if (i != 1) {
            VuSinCosDeg(&s, &c, dbgFldMoveCursorDir);
            dx = s * 300.0f;
            dz = -c * 300.0f;
            v[0].x += dx;
            v[0].z += dz;
            v[1].x -= dx;
            v[1].z -= dz;
        } else {
            v[0].y -= 300.0f;
            v[1].y += 300.0f;
        }
        GphLine3DDraw(2, v, cols[i], 1);
        FldDebugDrawBox(&v[1], &size, cols[i]);
    }
    FldDebugDrawBox(&dbgFldMoveCursorPos, &size, 0x80FFFFFF);
}
