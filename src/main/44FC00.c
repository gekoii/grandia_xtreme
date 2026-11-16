/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "ptcl.h"
#include "protos/main.h"

typedef struct {
    s8 c[4];
} Col;

typedef struct {
    Col v[4];
} Quad; /* 0x10 */

typedef struct {
    Quad q[32];
} Row; /* 0x200 */

#define SET4A(q, ch, _v) \
    { \
        (q)->v[3].c[ch] = (s32)_v; \
        (q)->v[2].c[ch] = (s32)_v; \
        (q)->v[1].c[ch] = (s32)_v; \
        (q)->v[0].c[ch] = (s32)_v; \
    }

#define LERP(a, b) (((f32)(b) - (f32)(a)) * t + (f32)(a))

#define SET4(q, ch, val) \
    { \
        s32 _v = (val); \
        (q)->v[3].c[ch] = (s32)_v; \
        (q)->v[2].c[ch] = (s32)_v; \
        (q)->v[1].c[ch] = (s32)_v; \
        (q)->v[0].c[ch] = (s32)_v; \
    }

void PtclSpriteColorAnim(PtclGen *o) {
    PtclGenDef *p;
    s32 *cnt;
    s32 i;
    s32 j;
    PtclSpriteKey *e;
    s32 tm;
    f32 t;
    Quad *q;
    f32 v;

    p = &o->def;
    cnt = o->groupLive;
    for (i = 0; i < o->groupCount; i++, cnt++) {
        if (*cnt > 0) {
            e = ((PtclSpriteKey (*)[31])o->keys)[i];
            for (j = 0; j < 31; j++, e++) {
                if (e->life == 0) {
                    continue;
                }
                tm = e->time - 1;
                if (tm < p->colorC1) {
                    if (p->colorCurve[0] == 0) {
                        continue;
                    }
                    t = PtclCurveEval(p->colorCurve[0], (f32)tm / (f32)p->colorC1);
                    v = LERP(p->colorKeys[3], p->colorKeys[7]);
                    q = &((Row *)o->colorPkt)[i].q[j + 1];
                    SET4A(q, 3, v);
                    if (!(o->flags & 0x100)) {
                        continue;
                    }
                    SET4(q, 0, LERP(p->colorKeys[0], p->colorKeys[4]));
                    SET4(q, 1, LERP(p->colorKeys[1], p->colorKeys[5]));
                    SET4(q, 2, LERP(p->colorKeys[2], p->colorKeys[6]));
                } else if (tm < p->colorC2) {
                    if (p->colorCurve[1] == 0) {
                        continue;
                    }
                    t = PtclCurveEval(p->colorCurve[1], (f32)(tm - p->colorC1) / (f32)(p->colorC2 - p->colorC1));
                    v = LERP(p->colorKeys[7], p->colorKeys[11]);
                    q = &((Row *)o->colorPkt)[i].q[j + 1];
                    SET4A(q, 3, v);
                    if (!(o->flags & 0x100)) {
                        continue;
                    }
                    SET4(q, 0, LERP(p->colorKeys[4], p->colorKeys[8]));
                    SET4(q, 1, LERP(p->colorKeys[5], p->colorKeys[9]));
                    SET4(q, 2, LERP(p->colorKeys[6], p->colorKeys[10]));
                } else {
                    if (p->colorCurve[2] == 0) {
                        continue;
                    }
                    t = PtclCurveEval(p->colorCurve[2], (f32)(tm - p->colorC2) / (f32)(e->life - p->colorC2));
                    v = LERP(p->colorKeys[11], p->colorKeys[15]);
                    q = &((Row *)o->colorPkt)[i].q[j + 1];
                    SET4A(q, 3, v);
                    if (!(o->flags & 0x100)) {
                        continue;
                    }
                    SET4(q, 0, LERP(p->colorKeys[8], p->colorKeys[12]));
                    SET4(q, 1, LERP(p->colorKeys[9], p->colorKeys[13]));
                    SET4(q, 2, LERP(p->colorKeys[10], p->colorKeys[14]));
                }
            }
        }
    }
}
