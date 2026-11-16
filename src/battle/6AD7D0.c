#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "sys.h"

/* PtclBurstType +0x10 (scale keys) and +0x30 (colour keys) as blocks */
typedef struct {
    f32 v[4];
    s16 t0;
    s16 t1;
    f32 r[3];
} SKeys;

typedef struct {
    u8 c[4][4];
    s16 t0;
    s16 t1;
    f32 r[3];
} CKeys;


void PtclBtlSprDraw(PtclBtlSpr *p, s32 n, s32 type, f32 alpha) {
    SprRect rect;
    GphAnimSet *tex;
    const SKeys *sk;
    const CKeys *ck;
    const PtclBurstType *pt;
    s32 life;
    PtclCellUv *e;
    GphAnimSet **q;

    sk = (const SKeys *)ptclBurstTypeTable[type].scale;
    ck = (const CKeys *)ptclBurstTypeTable[type].color;
    q = ptclBurstTypeTable[type].animSet;
    do {} while (0); /* do-while(0): block boundary after the table loads (register choice) */
    tex = NULL;
    if (q != NULL) {
        tex = *q;
    }
    GphAnimSetSelect(tex);
    pt = &ptclBurstTypeTable[type];
    GphTexBind(pt->tex);
    SprBegin(0.0f, 0.0f);
    sprState.blend = pt->unk2;
    for (; n > 0; n--, p++) {
        life = p->life;
        if (life > 0) {
            if (sk->t0 < p->life) {
                sprState.sx = (sk->v[0] - sk->v[1]) * ((f32)(life - sk->t0) * sk->r[0]) + sk->v[1];
            } else if (sk->t1 < p->life) {
                sprState.sx = (sk->v[1] - sk->v[2]) * ((f32)(life - sk->t1) * sk->r[1]) + sk->v[2];
            } else {
                sprState.sx = (sk->v[2] - sk->v[3]) * ((f32)life * sk->r[2]) + sk->v[3];
            }
            sprState.sy = sprState.sx = sprState.sx * p->scale;
            if (p->flags & 1) {
                sprState.sx = -sprState.sx;
            }
            if (p->flags & 2) {
                sprState.sy = -sprState.sy;
            }
            if (ck->t0 < p->life) {
                f32 k = (f32)(p->life - ck->t0) * ck->r[0];
                sprState.col[0].r = (ck->c[0][0] - ck->c[1][0]) * k + ck->c[1][0];
                sprState.col[0].g = (ck->c[0][1] - ck->c[1][1]) * k + ck->c[1][1];
                sprState.col[0].b = (ck->c[0][2] - ck->c[1][2]) * k + ck->c[1][2];
                sprState.col[0].a = (ck->c[0][3] - ck->c[1][3]) * k + ck->c[1][3];
            } else if (ck->t1 < p->life) {
                f32 k = (f32)(p->life - ck->t1) * ck->r[1];
                sprState.col[0].r = (ck->c[1][0] - ck->c[2][0]) * k + ck->c[2][0];
                sprState.col[0].g = (ck->c[1][1] - ck->c[2][1]) * k + ck->c[2][1];
                sprState.col[0].b = (ck->c[1][2] - ck->c[2][2]) * k + ck->c[2][2];
                sprState.col[0].a = (ck->c[1][3] - ck->c[2][3]) * k + ck->c[2][3];
            } else {
                f32 k = (f32)p->life * ck->r[2];
                sprState.col[0].r = (ck->c[2][0] - ck->c[3][0]) * k + ck->c[3][0];
                sprState.col[0].g = (ck->c[2][1] - ck->c[3][1]) * k + ck->c[3][1];
                sprState.col[0].b = (ck->c[2][2] - ck->c[3][2]) * k + ck->c[3][2];
                sprState.col[0].a = (ck->c[2][3] - ck->c[3][3]) * k + ck->c[3][3];
            }
            sprState.col[0].a = sprState.col[0].a * alpha;
            if (tex != 0) {
                e = GphAnimGetCellUv(p->anim.cell);
                rect.w = 2;
                rect.h = 2;
                rect.u0 = e->u0 / 8 + 1;
                rect.v0 = e->v0 / 8 + 1;
                rect.u1 = e->u1 / 8 + 1;
                rect.v1 = e->v1 / 8 + 1;
                SprDrawRect3D(&rect, -1, -1, p->pos);
            } else {
                SprDrawRect3D(&ptclGlowRect, -1, -1, p->pos);
            }
        }
    }
    SprFlush();
}
