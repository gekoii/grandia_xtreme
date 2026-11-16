#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR center;
    FVECTOR half;
    FVECTOR end;
} Box;

typedef struct {
    u8 pad[0x10];
    FVECTOR pos;
} Obj;

#define VADD(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void MapCollHitTreasures(Obj *, f32 *, f32);
void MapCollPushChars(Obj *, f32 *, f32);
void MapCollHitAmapSpots(Obj *, f32 *, f32);

s32 MapCollMove(void *arg0, Obj *obj, f32 *vel, void *arg3, s32 arg4, s32 flags, f32 f)
{
    Box box;
    f32 rad;
    s32 ret;
    s32 f4;
    s32 f2;

    ret = 0;
    box.half[0] = vel[0] * 0.5f;
    box.half[1] = vel[1] * 0.5f;
    box.half[2] = vel[2] * 0.5f;
    box.half[3] = 0.0f;
    VADD(box.center, box.half, obj->pos);
    VADD(box.end, obj->pos, vel);
    rad = fsqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]) * 0.5f;
    f4 = flags & 4;
    MapBorderClearHits();
    MapWallSelectLayer(0);
    MapCollHitTreasures(obj, vel, f);
    if (f4) {
        MapHtaGatherMove(&box, rad + f);
    }
    if (flags & 1) {
        MapCollPushChars(obj, vel, f);
    }
    if (f4) {
        MapCollHitAmapSpots(obj, vel, f + 5.0f);
    }
    f2 = flags & 2;
    if (f2) {
        if (arg4 == 0xFF) {
            MapBorderGather(&box, rad + f);
        } else {
            MapBorderGatherSegments(&box, arg3, arg4, rad + f);
        }
        if (f2) {
            MapBorderSlide(vel, obj->pos, f);
        }
    }
    if (f4) {
        ret = MapHtaResolve(vel, obj->pos, f);
    }
    if (arg0 && (flags & 8)) {
        MapPolyGatherModel(arg0, obj->pos, vel, rad + f);
        MapPolyResolve(vel, obj->pos, f);
    }
    if (f2) {
        MapBorderSlide(vel, obj->pos, f);
    }
    return ret;
}
