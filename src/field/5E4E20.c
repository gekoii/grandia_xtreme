/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

typedef struct {
    u8 pad[0x50];
    VuVec4 a;
    VuVec4 b;
    f32 dirx;
    f32 unk74;
    f32 dirz;
    u8 pad7C[0x9C - 0x7C];
    f32 radius;
} Mover;

void VuVecLerp(VuVec4 *, VuVec4 *, VuVec4 *, f32);

s32 MapBorderCountFacingWalls(s32 *arg0, Mover *arg1) {
    VuVec4 mid;
    s32 i;
    s32 cnt;
    f32 half;
    MapBorderWall *w;
    s32 n;

    half = 0.5f;
    MapBorderClearHits();
    VuVecLerp(&mid, &arg1->a, &arg1->b, half);
    MapBorderGather(&mid, arg1->radius * half);
    cnt = 0;
    n = mapBorderHitList->count;
    for (i = 0; i < n; i++) {
        w = &mapBorderHitList->hits[i].wall;
        if (mapBorderHitList->hits[i].type != 0) {
            if (arg1->dirz * w->dx - arg1->dirx * w->dz > 0.0f) {
                cnt++;
            }
        }
    }
    arg0[0] = 0;
    arg0[1] = 0;
    arg0[2] = 0;
    return cnt;
}
