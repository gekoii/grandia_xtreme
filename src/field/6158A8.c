/* cc: ee-gcc2.9-991111-01 */
/* rodata: 0x00674C90-0x00674CD0 */
#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec;

extern void VuVecCopy(Vec *, Vec *);

s32 AmapFindRandomRunEnd(Vec *out, Vec *pos) {
    s32 dirs[8][2] = {
        { -1, -1 }, { -1, 0 }, { -1, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, -1 }, { 0, 1 },
    };
    s32 z;
    s32 x;
    s32 i;
    s32 found;

    x = (s32)((pos->x - 250.0f) / 500.0f) + 1;
    z = (s32)-((pos->z + 250.0f) / 500.0f - 1.0f);
    VuVecCopy(out, pos);
    found = -1;
    for (i = 0; i < 8; i++) {
        if (AmapGetCellAttr(&amapWork, z + dirs[i][1], x + dirs[i][0]) == 1 && (VuRandInt(5) & 1)) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        return 0;
    }
    for (i = 0; i < 0xFF; i++) {
        if (AmapGetCellAttr(&amapWork, z + dirs[found][1], x + dirs[found][0]) != 1) {
            break;
        }
        x += dirs[found][0];
        z += dirs[found][1];
    }
    out->x = x * 0x1F4 - 0xFA;
    out->z = 0xFA - z * 0x1F4;
    return 1;
}
