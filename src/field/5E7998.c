/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x006737F8-0x00673800 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
} Obj;

typedef struct {
    Obj *obj;
    u8 pad4[0x6];
    u8 unkA;
    u8 padB[0x5];
    f32 dist;
} Ent;

typedef struct {
    u8 pad0[0xE2];
    s16 unkE2;
} Party;


static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define VSUB(d, a, b) \
    __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvsub.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" \
                     : : "r"(d), "r"(a), "r"(b) : "memory")

void MapCollHitAmapSpots(Obj *self, void *arg1, f32 r)
{
    FVECTOR d;
    Ent *e;
    Party *q;
    Obj *o;
    s32 n;
    s32 i;
    double dy;
    f32 len;

    n = FldSpotGetCount();
    e = FldSpotGetList();
    if (e == NULL) {
        return;
    }
    q = MapSetFileGet();
    if (q == NULL) {
        return;
    }
    for (i = 0; i < n; i++) {
        if (e->unkA != 0) {
            VSUB(d, self->pos, e->obj->pos);
            o = e->obj;
            dy = self->pos[1] - o->pos[1];
            if (dy < 0) {
                dy = -dy;
            }
            if (dy > 2000.0) {
                continue;
            }
            len = fsqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
            e->dist = len;
            if (len < r + (f32)q->unkE2) {
                MapCollPushCircle(o->pos, arg1, self->pos, (f32)q->unkE2, 40.0f);
            }
        }
        e++;
    }
}
