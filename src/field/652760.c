/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define vscale(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

#define vadd(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

#define vsub(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

void MiniBallSweepWalls(MiniBall *arg0, MiniBallHit *arg1)
{
    FVECTOR t;
    FVECTOR b;
    FVECTOR a;
    FVECTOR p0;
    FVECTOR p1;
    FVECTOR seg;
    FVECTOR rel;
    FVECTOR q;
    FVECTOR w;
    FVECTOR v;
    FVECTOR s;
    FVECTOR out;
    FVECTOR dir;
    FVECTOR nrm;
    FVECTOR tmp;
    u32 n;
    u32 i;
    MiniBallWall *e;
    f32 h;
    f32 d;
    f32 len;
    f32 f;
    f32 res;
    f32 r;

    r = arg0->travel;
    copyq(b, arg0->vel);
    copyq(a, arg0->pos);
    copyq(p0, miniBallWork->vec0);
    copyq(p1, miniBallWork->vec0);
    copyq(nrm, miniBallWork->vec0);
    vscale(t, b, r);
    n = miniBallData->hdr->wallCount;
    e = miniBallData->walls;
    e--;
    for (i = 0; i < n; i++) {
        e++;
        p0[0] = e->x0;
        p0[2] = e->y0;
        p1[0] = e->x1;
        p1[2] = e->y1;
        vsub(seg, p1, p0);
        vsub(rel, a, p0);
        nrm[0] = seg[2];
        nrm[2] = -seg[0];
        VuVecNormalize(nrm, nrm);
        h = VuVecDot(rel, nrm);
        if (h < 0.0f) {
            h = -h;
            vscale(nrm, nrm, -1.0f);
        }
        d = -VuVecDot(t, nrm);
        if (d < 0.0f) continue;
        if (d == 0.0f) continue;
        if (d + 10.0f < h) continue;
        if (h <= 10.0f) {
            vsub(w, seg, rel);
            if (VuVecDot(rel, w) < 0.0f) {
                if (0.0f < VuVecDot(seg, b)) {
                    vsub(v, p0, a);
                } else {
                    vsub(v, p1, a);
                }
                res = MiniBallSweepCircle(v, b, 10.0f);
            } else {
                res = h - 10.0f;
            }
        } else {
            h = h - 10.0f;
            len = VuVecNormalizeLen(dir, seg);
            vscale(s, t, h / d);
            vadd(q, rel, s);
            f = VuVecDot(q, dir);
            if (f < 0.0f || len < f) {
                if (f < 0.0f) {
                    vsub(v, p0, a);
                } else {
                    vsub(v, p1, a);
                }
                res = MiniBallSweepCircle(v, b, 10.0f);
            } else {
                res = VuVecNormalizeLen(tmp, s);
            }
        }
        if (arg1->dist <= res) continue;
        if (res <= 0.0f) {
            copyq(out, miniBallWork->vec0);
        } else {
            vscale(out, b, res);
        }
        arg1->dist = res;
        arg1->index = i;
        arg1->kind = 0;
        copyq(arg1->side, dir);
        copyq(arg1->normal, nrm);
        copyq(arg1->move, out);
    }
}
