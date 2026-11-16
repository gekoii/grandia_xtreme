/* rodata: 0x006737C0-0x006737C8 */
#include "common.h"
#include "field/state.h"
#include "protos/field.h"

typedef struct {
    u8 pad[0x10];
    f32 unk10;
    f32 unk14;
} Vec;


void MapCollHitTreasures(Vec *arg0, void *arg1, f32 arg2) {
    FldTreasure *p;
    FldTreasure *base;
    s32 i;
    double d;

    base = FldTreasureGetWork();
    for (i = 0; i < 64; i++) {
        p = &base[i];
        if (p->state >= 3) {
            d = arg0->unk14 - p->path[3][1];
            if (d < 0) {
                d = -d;
            }
            if (d <= 2000.0) {
                MapCollPushCircle(p->path[3], arg1, &arg0->unk10, 40.0f, arg2);
            }
        }
    }
}
