#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR center;
    FVECTOR half;
    FVECTOR end;
    FVECTOR vel;
} Box;

typedef struct {
    s32 hit;
    u8 pad4[8];
    f32 unkC;
    FVECTOR pos;
    FVECTOR vel;
    FVECTOR a;
    FVECTOR b;
    u8 pad50[0x30];
} Hit;

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

extern s32 mapCollFlags;

void VuVecCopy(f32 *, f32 *);
s32 MapPolySweep(Hit *);

s32 MapCollMoveCamera(f32 *pos, f32 *vel, f32 f)
{
    Box box;
    Hit st;
    f32 rad;
    s32 res;
    s32 ret;

    VuVecCopy(st.pos, pos);
    VuVecCopy(st.vel, vel);
    VuVecCopy(box.vel, vel);
    st.unkC = f;
    st.hit = 0;
    box.half[0] = vel[0] * 0.5f;
    box.half[1] = vel[1] * 0.5f;
    box.half[2] = vel[2] * 0.5f;
    box.half[3] = 0.0f;
    VADD(box.center, box.half, pos);
    VADD(box.end, pos, vel);
    rad = fsqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]) * 0.5f;
    MapBorderClearHits();
    MapPolyClear();
    res = 0;
    MapWallSelectLayer(1);
    MapPolyGetList();
    if (mapCollFlags & 2) {
        MapBorderGather(&box, rad + f);
        MapBorderSlide(vel, pos, f);
        res |= st.hit;
        st.hit = 0;
    }
    if (mapCollFlags & 8) {
        if (MapCollModelGet(1)) {
            MapPolyGatherModel(MapCollModelGet(1), pos, vel, rad + f);
            MapPolySweep(&st);
            if (st.hit) {
                VADD(vel, st.a, st.b);
                vel[3] = 0.0f;
                res = st.hit;
                st.hit = 0;
            }
        }
    }
    if (mapCollFlags & 4) {
        MapHtaResolve(vel, pos, f);
        res |= st.hit;
        st.hit = 0;
    }
    if (mapCollFlags & 2) {
        MapBorderSlide(vel, pos, f);
    }
    ret = 0;
    if (res) {
        ret = 1;
        st.hit = 1;
    }
    return ret;
}
