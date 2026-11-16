/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "main/n10.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    VuVec3 *pos;
} Obj;


void SndPlayAtPos(Obj *arg0) {
    SndPlayReq m;
    f32 dx;
    f32 dy;
    f32 dz;

    dx = arg0->pos->x - sndListenerPos.x;
    dy = arg0->pos->y - sndListenerPos.y;
    dz = arg0->pos->z - sndListenerPos.z;
    m.index = arg0->unk4;
    m.handle = arg0->unk0;
    m.dist = sqrtf(dx * dx + dy * dy + dz * dz) * 256.0f / sndDistanceScale;
    m.hPan = (SndCalcAngle(dx, dz) - sndListenerAngle) & 0xFF;
    SndPlay(&m);
}
