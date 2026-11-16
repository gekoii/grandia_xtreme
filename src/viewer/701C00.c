/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

void VuSinCosDeg(f32 *, f32 *, f32);
void VuVecCopy(f32 *, VwrFlockAgent *);

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/* VU0 helpers: unit vector (vf0), and a - b on xyz (stored separately or in one statement). */
#define vunit(v) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(v) : "memory")

#define vsub(d, a, b)                                \
    {                                                \
        __asm__ volatile(                            \
            "lqc2 $vf1, 0(%0)\n\t"                   \
            "lqc2 $vf2, 0(%1)\n\t"                   \
            "vsub.xyz $vf1, $vf1, $vf2"              \
            : : "r"(a), "r"(b) : "memory");          \
        __asm__ volatile("sqc2 $vf1, 0(%0)" : : "r"(d) : "memory"); \
    }

#define vsubq(d, a, b)                               \
    __asm__ volatile(                                \
        "lqc2 $vf1, 0(%1)\n\t"                       \
        "lqc2 $vf2, 0(%2)\n\t"                       \
        "vsub.xyz $vf1, $vf1, $vf2\n\t"              \
        "sqc2 $vf1, 0(%0)"                           \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define WRAP(x)                     \
    if ((x) < 0.0f) {               \
        (x) += 6.283184f;           \
    } else if ((x) >= 6.283184f) {  \
        (x) -= 6.283184f;           \
    }

/* Step a part forward along its heading by its speed. */
#define MOVE(p)                                                     \
    {                                                               \
        VuSinCosDeg(&s, &c, (p)->heading / 3.141592f * 180.0f);       \
        v.x = -s * -(p)->speed;                                      \
        v.y = 0.0f;                                                 \
        v.z = c * -(p)->speed;                                       \
        d = fsqrt(v.x * v.x + v.y * v.y + v.z * v.z);               \
        v.x /= d;                                                   \
        v.y /= d;                                                   \
        v.z /= d;                                                   \
        (p)->pos.x += v.x * d;                                          \
        (p)->pos.y += v.y * d;                                          \
        (p)->pos.z += v.z * d;                                          \
    }

void VwrFlockUpdate(void)
{
    VwrVec4 v;
    VwrVec4 a;
    VwrVec4 b;
    VwrVec4 t;
    f32 s;
    f32 c;
    f32 ang;
    f32 d;
    s32 i;
    s32 j;
    s32 cnt;
    GphModel *o;

    for (i = 0; i < vwrFlockCfg.count; i++) {
        for (j = i + 1; j < vwrFlockCfg.count; j++) {
            vwrFlockAgents[i].dist[j] = VwrVecDistance(&vwrFlockAgents[j], &vwrFlockAgents[i]);
        }
        vwrFlockAgents[i].dist[i] = 0.0f;
    }

    for (i = 0; i < vwrFlockCfg.count; i++) {
        if (vwrFlockAgents[i].speed < 4.0f) {
            vwrFlockAgents[i].slow = 1;
        } else {
            vwrFlockAgents[i].slow = 0;
        }
    }

    for (i = 0; i < vwrFlockCfg.count; i++) {
        cnt = 0;
        vwrFlockAgents[i].avoiding = 0;
        for (j = i + 1; j < vwrFlockCfg.count; j++) {
            if (vwrFlockAgents[i].dist[j] < vwrFlockAgents[i].sepRadius) {
                vwrFlockAgents[i].avoiding = 1;
                vunit(&a);
                vunit(&b);
                vsubq(&a, &vwrFlockAgents[j], &vwrFlockAgents[i]);
                VwrDirToRotXY(&a, &b);
                b.y = VwrAngleDiff(vwrFlockAgents[i].heading, b.y);
                ang = b.y / 3.141592f * 180.0f + 180.0f;
                if (ang < 0.0f) {
                    ang += 360.0f;
                }
                if (ang > 360.0f) {
                    ang -= 360.0f;
                }
                if (ang < 180.0f) {
                    if (ang < 179.0f) {
                        cnt--;
                    }
                } else if (ang < 359.0f) {
                    cnt++;
                }
            }
        }
        if (vwrFlockAgents[i].avoiding) {
            vwrFlockAgents[i].avoidTimer = 16;
            if (cnt) {
                if (cnt < 0) {
                    vwrFlockAgents[i].heading -= 0.017453289f;
                } else {
                    vwrFlockAgents[i].heading += 0.017453289f;
                }
                WRAP(vwrFlockAgents[i].heading);
            }
            vwrFlockAgents[i].speed -= vwrFlockAgents[i].accel;
            if (vwrFlockAgents[i].speed < vwrFlockAgents[i].minSpeed) {
                vwrFlockAgents[i].speed = vwrFlockAgents[i].minSpeed;
            }
            MOVE(&vwrFlockAgents[i]);
        }
    }

    t.x = vwrFlockWaypoints.v[vwrFlockWaypoints.cur].x;
    t.y = vwrFlockWaypoints.v[vwrFlockWaypoints.cur].y;
    t.z = vwrFlockWaypoints.v[vwrFlockWaypoints.cur].z;
    for (i = 0; i < vwrFlockCfg.count; i++) {
        if (vwrFlockAgents[i].avoiding == 0 && vwrFlockAgents[i].avoidTimer != 0) {
            vwrFlockAgents[i].avoidTimer--;
            d = VwrVecDistance(&t, &vwrFlockAgents[i]);
            vwrFlockAgents[i].speed -= 0.1f;
            if (vwrFlockAgents[i].speed < vwrFlockAgents[i].minSpeed) {
                vwrFlockAgents[i].speed = vwrFlockAgents[i].minSpeed;
            }
            WRAP(vwrFlockAgents[i].heading);
            if (d > 200.0f) {
                MOVE(&vwrFlockAgents[i]);
            }
        } else if (vwrFlockAgents[i].avoiding == 0) {
            d = VwrVecDistance(&t, &vwrFlockAgents[i]);
            if (d > 200.0f) {
                if (d > 800.0f) {
                    vwrFlockAgents[i].speed += vwrFlockAgents[i].accel;
                    if (vwrFlockAgents[i].maxSpeed < vwrFlockAgents[i].speed) {
                        vwrFlockAgents[i].speed = vwrFlockAgents[i].maxSpeed;
                    }
                } else {
                    vwrFlockAgents[i].speed -= vwrFlockAgents[i].accel;
                    if (vwrFlockAgents[i].speed < vwrFlockAgents[i].minSpeed) {
                        vwrFlockAgents[i].speed = vwrFlockAgents[i].minSpeed;
                    }
                }
                if (vwrFlockAgents[i].pos.y < t.y - 10.0f) {
                    vwrFlockAgents[i].climb += vwrFlockAgents[i].climbAccel;
                    if (vwrFlockAgents[i].climb >= 1.5f) {
                        vwrFlockAgents[i].climb = 1.5f;
                    }
                    vwrFlockAgents[i].climbed = 1;
                    vwrFlockAgents[i].pos.y += vwrFlockAgents[i].climb;
                } else if (t.y + 10.0f < vwrFlockAgents[i].pos.y) {
                    vwrFlockAgents[i].climb -= vwrFlockAgents[i].climbAccel;
                    if (vwrFlockAgents[i].climb <= -1.5f) {
                        vwrFlockAgents[i].climb = -1.5f;
                    }
                    vwrFlockAgents[i].climbed = 1;
                    vwrFlockAgents[i].pos.y += vwrFlockAgents[i].climb;
                }
                vunit(&a);
                vunit(&b);
                vsub(&a, &t, &vwrFlockAgents[i]);
                VwrDirToRotXY(&a, &b);
                b.y = VwrAngleDiff(vwrFlockAgents[i].heading, b.y);
                ang = b.y / 3.141592f * 180.0f;
                if (ang < 0.0f) {
                    ang += 360.0f;
                }
                if (ang > 360.0f) {
                    ang -= 360.0f;
                }
                if (ang < 180.0f) {
                    if (ang < 179.0f) {
                        if (ang - 2.0f <= 0.0f) {
                            vwrFlockAgents[i].heading = 0.0f;
                        } else {
                            vwrFlockAgents[i].heading -= 0.017453289f;
                        }
                    }
                } else if (ang < 359.0f) {
                    if (ang + 2.0f >= 360.0f) {
                        vwrFlockAgents[i].heading = 6.283184f;
                    } else {
                        vwrFlockAgents[i].heading += 0.017453289f;
                    }
                }
                WRAP(vwrFlockAgents[i].heading);
                if (d > 200.0f) {
                    MOVE(&vwrFlockAgents[i]);
                }
            } else {
                vwrFlockWaypoints.cur++;
                if (vwrFlockCfg.waypointCount == vwrFlockWaypoints.cur) {
                    vwrFlockWaypoints.cur = 0;
                }
            }
        }
    }

    for (i = 0; i < vwrFlockCfg.count; i++) {
        if (vwrFlockAgents[i].climbed) {
            vwrFlockAgents[i].climbed = 0;
            if (vwrFlockAgents[i].pos.y >= 620.0f) {
                if (!vwrFlockAgents[i].below) {
                    vwrFlockAgents[i].unk240 = 30;
                    vwrFlockAgents[i].speed *= 0.5f;
                    vwrFlockAgents[i].below = 1;
                    vwrFlockAgents[i].climb = 0.0f;
                }
            } else if (vwrFlockAgents[i].below) {
                vwrFlockAgents[i].unk240 = 30;
                vwrFlockAgents[i].speed *= 0.5f;
                vwrFlockAgents[i].below = 0;
                vwrFlockAgents[i].climb = 0.0f;
            }
        }
    }

    for (i = 0; i < vwrFlockCfg.count; i++) {
        o = vwrModelList.items[vwrFlockCfg.firstModel + i];
        VuVecCopy(&o->pos, &vwrFlockAgents[i]);
        o->rot[0] = vwrFlockAgents[i].pitch * 57.295776f * 182.04445f;
        o->rot[1] = -(vwrFlockAgents[i].heading * 57.295776f * 182.04445f);
        o->rot[2] = vwrFlockAgents[i].roll * 57.295776f * 182.04445f;
    }
}
