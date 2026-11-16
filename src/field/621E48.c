/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

typedef int s128 __attribute__((mode(TI)));

typedef union {
    s128 q;
    f32 f[4];
} Q;


void VuBezier3(Q *dst, Q *m, f32 t);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void FldMenuWinUpdate(void) {
    Q m[4];
    Q r;
    FldMenuWin *p;
    FldMenuQuad *q;
    s32 i;
    s32 j;
    s32 done;
    s32 a;

    fldMenuCursor->t += 0.01f;
    if (fldMenuCursor->t >= 1.0f) {
        fldMenuCursor->t = 0.0f;
    }
    copyq(&m[0], (Q *)fldMenuCursor->colorKeys[0]);
    copyq(&m[1], (Q *)fldMenuCursor->colorKeys[2]);
    copyq(&m[2], (Q *)fldMenuCursor->colorKeys[2]);
    copyq(&m[3], (Q *)fldMenuCursor->colorKeys[0]);
    VuBezier3((Q *)fldMenuCursor->colorKeys[4], m, fldMenuCursor->t);
    copyq(&m[0], (Q *)fldMenuCursor->colorKeys[1]);
    copyq(&m[1], (Q *)fldMenuCursor->colorKeys[3]);
    copyq(&m[2], (Q *)fldMenuCursor->colorKeys[3]);
    copyq(&m[3], (Q *)fldMenuCursor->colorKeys[1]);
    VuBezier3((Q *)fldMenuCursor->colorKeys[5], m, fldMenuCursor->t);
    if (fldMenuCursor->shown && (fldMenuCursor->fromX != fldMenuCursor->x || fldMenuCursor->fromY != fldMenuCursor->y)) {
        MenuCursorMoveTo(fldMenuCursor->x, fldMenuCursor->y);
        fldMenuCursor->fromX = fldMenuCursor->x;
        fldMenuCursor->fromY = fldMenuCursor->y;
    }
    for (i = 0; i < 18; i++) {
        p = &fldMenuWins[i];
        if (p->active == 0) {
            continue;
        }
        switch (p->state) {
        case 1:
        case 5:
            if (p->timer == 0) {
                p->state++;
            } else {
                p->timer--;
            }
            break;
        case 2:
            copyq(&m[0], &p->openStart);
            copyq(&m[1], &p->openEnd);
            copyq(&m[2], &p->openEnd);
            copyq(&m[3], &p->openEnd);
            VuBezier3(&p->pos, m, p->unk34);
            if (p->alpha < 0.8f) {
                p->alpha += 0.05f;
                if (p->alpha > 0.8f) {
                    p->alpha = 0.8f;
                }
            }
            if (p->unk34 < 1.0f) {
                p->unk34 += 0.08f;
                if (p->unk34 > 1.0f) {
                    p->unk34 = 1.0f;
                }
            }
            if (p->unk34 == 1.0f && p->alpha == 0.8f) {
                p->unk34 = 0.0f;
                p->state++;
            }
            break;
        case 3:
            done = 0;
            if (p->flags & 0x4000) {
                if (p->unk34 < 1.0f) {
                    p->unk34 += 0.06f;
                    if (p->unk34 > 1.0f) {
                        p->unk34 = 1.0f;
                    } else {
                        done = 1;
                    }
                } else {
                    p->unk34 = 1.0f;
                }
                ((f32 *)m)[0] = 0.0f;
                ((f32 *)m)[4] = p->unk24;
                ((f32 *)m)[8] = p->unk24;
                ((f32 *)m)[12] = p->unk24;
                VuBezier3(&r, m, p->unk34);
                p->h2 = r.f[0] + 0.5f;
            }
            if (!done) {
                p->unk34 = 0.0f;
                p->state++;
            }
            break;
        case 6:
            copyq(&m[0], &p->openStart);
            copyq(&m[1], &p->openStart);
            copyq(&m[2], &p->openStart);
            copyq(&m[3], &p->openEnd);
            VuBezier3(&p->pos, m, p->unk34);
            if (p->alpha > 0.0f) {
                p->alpha -= 0.05f;
                if (p->alpha < 0.0f) {
                    p->alpha = 0.0f;
                }
            }
            if (p->unk34 < 1.0f) {
                p->unk34 += 0.08f;
                if (p->unk34 > 1.0f) {
                    p->unk34 = 1.0f;
                }
            }
            if (p->unk34 == 1.0f && p->alpha == 0.0f) {
                p->state = 0;
                p->active = 0;
            }
            break;
        }
        if (p->unk16 != 0) {
            q = &p->quads[0x50];
            q->type = 1;
            q->v[0].r = 0x80;
            q->v[0].g = 0x80;
            q->v[0].b = 0x80;
            q[1].type = 0;
            q->z = 0;
            q->v[0].a = p->alpha * 160.0f;
            q->v[0].x = p->x;
            q->v[0].y = p->y;
            q->v[1].x = p->x + p->w;
            q->v[1].y = p->y + p->h;
            q->v[0].u = 0;
            q->v[0].v = 0;
            q->v[1].u = 0x60;
            q->v[1].v = 0x60;
        }
        p->unk38 += 0.02f;
        if (p->unk38 >= 1.0f) {
            p->unk38 -= 1.0f;
        }
        p->unk3C += 0.01f;
        if (p->unk3C >= 1.0f) {
            p->unk3C -= 1.0f;
        }
        for (j = 0; j < 1; j++) {
            if (p->layerTex[j] && p->layerQuad[j] && p->layerKind[j] == 1) {
                for (q = p->layerQuad[j]; q->type; q++) {
                    a = p->alpha * (p->layerVal[j] / 0.8f);
                    q->v[0].a = a;
                    q->v[1].a = a;
                    q->v[2].a = a;
                    q->v[3].a = a;
                }
            }
        }
        if (p->glyphCount != 0) {
            q = &p->quads[0x52];
            for (j = 0; j < p->glyphCount; j++, q++) {
                q->v[0].a = p->alpha * 160.0f;
            }
            q->type = 0;
        }
        for (q = &p->quads[0]; q->type; q++) {
            a = p->alpha * 160.0f;
            q->v[0].a = a;
            q->v[1].a = a;
            q->v[2].a = a;
            q->v[3].a = a;
        }
        for (j = 1; j < 16; j++) {
            if (p->layerTex[j] && p->layerQuad[j] && p->layerKind[j] == 1) {
                for (q = p->layerQuad[j]; q->type; q++) {
                    a = p->alpha * (p->layerVal[j] / 0.8f);
                    q->v[0].a = a;
                    q->v[1].a = a;
                    q->v[2].a = a;
                    q->v[3].a = a;
                }
            }
        }
    }
    MenuHelpUpdate();
    MenuWidgetsUpdate();
}
