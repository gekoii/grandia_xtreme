/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec;

typedef struct {
    f32 pos[3];
    f32 rot;
    f32 vel[3];
    f32 rotv;
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
    f32 life;
    f32 f34;
    f32 f38;
    u8 pad3C[3];
    u8 active;
} Part;

typedef struct {
    Vec pos;
    Vec ofs;
    u8 pad20[0x20];
} Part2;

typedef struct {
    u8 c[4][4];
} Col;

typedef struct {
    u8 a;
    u8 pad[3];
} A4;

typedef struct {
    A4 v[4];
} ColA;

typedef struct {
    Part p[31];
    u8 pad7C0[0x2C];
    f32 f7EC;
    u8 pad7F0[0x20];
    Vec v810;
    u8 pad820[0x20];
    Vec v840;
    Vec v850;
    u8 pad860[0x30];
    Part2 p2[31];
    u8 pad1050[0x50];
    Col col[31];
    f32 f1290;
    f32 f1294;
    f32 f1298;
    u8 pad129C[8];
    f32 f12A4;
    f32 f12A8;
    u8 pad12AC[4];
    u8 f12B0;
    u8 count;
    s8 f12B2;
    u8 f12B3;
    u8 f12B4;
    u8 f12B5;
    u8 f12B6;
    u8 f12B7;
    u8 f12B8;
    u8 f12B9;
    u8 f12BA;
} Emitter;

extern s32 ptclEmitterPause;

void VuSinCosDeg(f32 *, f32 *, f32);
s32 VuFloatToInt(f32);

#define RANDF() (((f32)rand() - (f32)rand()) * 2.3283064e-10f)

#define VADD(d, a, b)                                   \
    __asm__ volatile(                                   \
        "lqc2 $vf1, 0(%1)\n\t"                          \
        "lqc2 $vf2, 0(%2)\n\t"                          \
        "vadd.xyz $vf1, $vf1, $vf2\n\t"                 \
        "sqc2 $vf1, 0(%0)"                              \
        : : "r"(d), "r"(a), "r"(b) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

s32 PtclEmitterUpdate(Emitter *em, s32 arg1)
{
    f32 ext[3];
    f32 sc[2];
    s32 alive;
    s32 i;
    f32 fade;
    f32 t0, t1, t2;
    f32 saved;
    s8 a;
    f32 m0, m1, m2;
    Part *parts;
    Part2 *pp;
    ColA *ca;

    alive = 0;
    pp = em->p2;
    if (em->count == 0) {
        return 10;
    }
    if (em->f12BA == 0) {
        return 20;
    }
    if (arg1 == 0) {
        return 30;
    }
    if (em->f12B0 == 0) {
        return 40;
    }
    if (ptclEmitterPause != 0) {
        return 50;
    }
    ca = (ColA *)&em->col[0].c[0][3];
    parts = em->p;
    em->f12A4 += 1.0f;
    ext[0] = ext[1] = ext[2] = 0.0f;
    fade = em->f12B3;
    for (i = 0; i < em->count; i++) {
        if (parts[i].active == 0) {
            continue;
        }
        alive = 1;
        if (parts[i].f34 > em->f12A4) {
            continue;
        }
        if (em->f12B8 != 5) {
            VADD(&pp[i].pos, &pp[i].pos, parts[i].pos);
            VADD(&pp[i].pos, &pp[i].pos, &em->v810);
        } else {
            parts[i].rot += parts[i].rotv;
            VuSinCosDeg(&sc[0], &sc[1], parts[i].rot);
            pp[i].pos.x = sc[1] * parts[i].pos[0];
            pp[i].pos.y = sc[0] * parts[i].pos[1];
            pp[i].pos.z = -sc[1] * parts[i].pos[2];
        }
        if (!(parts[i].f38 * (1.0f - em->f7EC) < parts[i].life - 1.0f)) {
            if (em->f12B8 == 3) {
                parts[i].vel[1] = 0.0f;
            }
            if (em->f12B8 == 4) {
                parts[i].vel[0] = 0.0f;
                parts[i].vel[1] = 0.0f;
                parts[i].vel[2] = 0.0f;
            }
        }
        VADD(parts[i].pos, parts[i].pos, parts[i].vel);
        parts[i].life -= 1.0f;
        if (parts[i].life < 0.0f) {
            parts[i].life = 0.0f;
            parts[i].active = 0;
        }
        switch (em->f12B7) {
        case 1:
            if (parts[i].f38 * 0.5f <= parts[i].life) {
                parts[i].f28 += parts[i].f2C;
                if (parts[i].f28 > 127.0f) {
                    parts[i].f28 = 127.0f;
                }
                break;
            }
        case 0:
            parts[i].f28 -= parts[i].f2C;
            break;
        case 2:
            if (parts[i].life <= fade) {
                parts[i].f28 -= parts[i].f2C;
            }
            break;
        }
        if (parts[i].f28 < 0.0f) {
            parts[i].f28 = 0.0f;
            parts[i].active = 0;
        }
        a = VuFloatToInt(parts[i].f28);
        a -= 127;
        ca[i].v[0].a = a;
        ca[i].v[1].a = a;
        ca[i].v[2].a = a;
        ca[i].v[3].a = a;
        if (parts[i].active) {
            pp[i].ofs.x += parts[i].f20;
            pp[i].ofs.y += parts[i].f24;
            t0 = pp[i].pos.x + pp[i].ofs.x;
            m0 = ext[0];
            ext[0] = (ext[0] < t0) ? t0 : m0;
            t1 = pp[i].pos.y + pp[i].ofs.y;
            m1 = ext[1];
            ext[1] = (ext[1] < t1) ? t1 : m1;
            t2 = pp[i].pos.z + pp[i].ofs.z;
            m2 = ext[2];
            ext[2] = (ext[2] < t2) ? t2 : m2;
        } else {
            pp[i].ofs.x = 0.0f;
            pp[i].ofs.y = 0.0f;
        }
    }
    if (em->f12B2 == -1) {
        saved = em->f12A4;
        for (i = 0; i < em->count; i++) {
            if (em->p[i].active == 0) {
                PtclEmitterSpawn(em, i & 0xFF);
                em->f12A4 += em->f1298 + em->f1298 * RANDF() * 0.5f;
            }
        }
        em->f12A4 = saved;
    }
    em->f12B9 = alive;
    if (alive) {
        em->v840.x += em->v850.x;
        em->v840.y += em->v850.y;
        em->v840.z += em->v850.z;
        em->v840.w += em->v850.w;
        em->f12A8 = fsqrt(ext[0] * ext[0] + ext[1] * ext[1] + ext[2] * ext[2]);
        em->f12BA = 1;
        return 1;
    }
    if (em->f12B2 == -1) {
        em->f12BA = 1;
        return 1;
    }
    em->f12BA = 0;
    return 0;
}
