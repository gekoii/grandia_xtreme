/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef struct Obj Obj;

typedef struct {
    u8 pad0[0x60];
    Obj *obj;
} Ent;

struct Obj {
    u8 pad0[0x10];
    f32 pos[4];
};

typedef struct {
    u8 pad0[0xC];
    u16 flags;
    u8 padE[0x60 - 0xE];
    Obj *obj;
} Actor;


void MapCollPushChars(Obj *a, f32 *vel, f32 r)
{
    Ent *p;
    Obj *o;
    Obj *q;
    Actor *e;
    s32 i;
    s32 n;
    f32 dx;
    f32 dz;

    p = ChrGetByNo(1);
    q = p->obj;
    if (q != a) {
        dx = q->pos[0] - a->pos[0];
        dz = q->pos[2] - a->pos[2];
        if (__builtin_fabsf(q->pos[1] - a->pos[1]) < 200.0f && __builtin_fabsf(dx * dx + dz * dz) > 2.0f && MapCollPushCircle(q->pos, vel, a->pos, 40.0f, r) == 1) {
            __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(vel) : "memory");
        }
    }
    n = ChrTenantGetCount();
    for (i = 0; i < n; i++) {
        e = ChrTenantAt(i);
        if (e == NULL) continue;
        if (e->flags & 0x10) continue;
        if (!(e->flags & 0x1000)) continue;
        if (!(e->flags & 2)) continue;
        o = e->obj;
        if (o == a) continue;
        if (!(__builtin_fabsf(o->pos[1] - a->pos[1]) < 200.0f)) continue;
        dx = o->pos[0] - a->pos[0];
        dz = o->pos[2] - a->pos[2];
        if (__builtin_fabsf(dx * dx + dz * dz) > 2.0f && MapCollPushCircle(o->pos, vel, a->pos, 40.0f, r) == 1 && p->obj != a) {
            __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(vel) : "memory");
        }
    }
}
