/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "party.h"
#include "sys.h"

extern SprRect ipDialRects[];
extern SprRect ipPartyPointerRect;
extern SprRect ipEnemyPointerRect;
extern SprRect ipEnemyRingRects[];
extern SprRectListEntry ipPartyIconSprList[];
extern SprRectListEntry ipEnemyIconSprList[];
extern SprRect ipComboMarkRect;
extern SprRect ipIconPartyRects[];
extern SprRect ipIconUnitLabelRects[];

void VuSinCosAngle(f32 *, f32 *, s32);
f32 VuSinAngle(s32);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

static inline BtlUnit *chr(s32 n) {
    return &btlUnits[n];
}

void IpGaugeDraw(void) {
    GphBatchQuad q[2];
    BtlUnit *list[16];
    f32 sc0, sc1;
    f32 a1, a0;
    f32 b1, b0;
    f32 c1, c0;
    f32 d1, d0;
    f32 e1, e0;
    GphBatchQuad *p;
    BtlUnit **lp;
    BtlUnit **pp;
    BtlUnit *ch;
    IpIcon *e;
    IpComboPreview *fx;
    IpComboPreview *fp;
    s32 n;
    s32 i;
    s32 j;
    s32 m;
    s32 jj;
    s32 k;
    s32 angA;
    s32 angB;
    f32 val;
    f32 pulse;
    f32 alpha;
    f32 sz;
    f32 scl;
    f32 t;
    s32 col;
    s32 col0;
    s32 start2;
    s32 start;
    s32 j2;
    BtlUnit **pp2;
    IpGauge *bt;

    GphTexBind(0xEF);
    p = q;
    q[1].type = 0;
    p->type = 1;
    p->v[0].r = 0x80;
    p->v[0].g = 0x80;
    p->v[0].b = 0x80;
    p->v[0].a = 0x80;
    p->v[1].r = 0x80;
    p->v[1].g = 0x80;
    p->v[1].b = 0x80;
    p->v[1].a = 0x80;
    p->v[2].r = 0x80;
    p->v[2].g = 0x80;
    p->v[2].b = 0x80;
    p->v[2].a = 0x80;
    p->v[3].r = 0x80;
    p->v[3].g = 0x80;
    p->v[3].b = 0x80;
    p->v[3].a = 0x80;
    p->v[2].v = 1;
    p->v[3].v = 0xE;
    p->v[0].v = 1;
    p->v[1].v = 0xE;
    VuSinCosAngle(&sc0, &sc1, 0xE000 - ipDialAng);
    p->v[2].u = p->v[3].u = ~btlTickCount & 0x3F;
    p->v[2].x = sc0 * 64.0f * ipDialScaleX + 542.0f + ipDialX;
    p->v[3].x = sc0 * 48.0f * ipDialScaleX + 542.0f + ipDialX;
    p->v[2].y = sc1 * 64.0f * ipDialScaleY + (ipGauge.slide + 344.0f) + ipDialY;
    p->v[3].y = sc1 * 48.0f * ipDialScaleY + (ipGauge.slide + 344.0f) + ipDialY;
    for (n = 1; n < 17; n++) {
        p->v[0].x = p->v[2].x;
        p->v[0].y = p->v[2].y;
        p->v[1].x = p->v[3].x;
        p->v[1].y = p->v[3].y;
        VuSinCosAngle(&sc0, &sc1, 0xE000 - ipDialAng - (n << 10));
        p->v[2].x = sc0 * 64.0f * ipDialScaleX + 542.0f + ipDialX;
        p->v[3].x = sc0 * 48.0f * ipDialScaleX + 542.0f + ipDialX;
        p->v[2].y = sc1 * 64.0f * ipDialScaleY + (ipGauge.slide + 344.0f) + ipDialY;
        p->v[3].y = sc1 * 48.0f * ipDialScaleY + (ipGauge.slide + 344.0f) + ipDialY;
        if (p->v[2].u >= 0x40) {
            p->v[2].u -= 0x40;
        }
        p->v[0].u = p->v[1].u = p->v[2].u;
        p->v[3].u = p->v[2].u = p->v[2].u + 4;
        GphBatchDrawQuads(q);
    }

    GphTexBind(0xED);
    SprBegin(ipDialX + 542.0f, ipGauge.slide + 344.0f + ipDialY);
    sprState.sx = ipDialScaleX;
    sprState.sy = ipDialScaleY;
    sprState.angle = ipDialAng;
    SprDrawRect(&ipDialRects[0], -0x78, -0x52);
    sprState.offX = 4;
    sprState.offY = -4;
    if (ipGauge.comboMark != 0) {
        col0 = (VuSinAngle(btlTickCount << 12) + 1.0f) * 32.0f * 0.5f + 96.0f;
        sprState.col[0].b = col0;
        sprState.col[0].g = col0;
        sprState.col[0].r = col0;
    } else {
        col0 = 0x60;
        sprState.col[0].b = col0;
        sprState.col[0].g = col0;
        sprState.col[0].r = col0;
    }
    SprDrawRect(&ipDialRects[1], -ipDialRects[1].w / 2, -ipDialRects[1].h / 2);
    sprState.offX = -0x5F;
    sprState.offY = 0x47;
    sprState.col[0].b = 0x80;
    sprState.col[0].g = 0x80;
    sprState.col[0].r = 0x80;
    SprDrawRect(&ipDialRects[2], -ipDialRects[2].w / 2, -ipDialRects[2].h / 2);
    if (0.0f < ipGauge.highlight) {
        sprState.offY = 0;
        sprState.offX = 0;
        sprState.col[0].a = ipGauge.highlight * 96.0f;
        SprPushTransform(0, -96.0f, 72.0f, ipGauge.highlight * ipGauge.highlight * 4.0f + 1.0f, ipGauge.highlight * ipGauge.highlight * 4.0f + 1.0f);
        SprDrawRect(&ipDialRects[2], -ipDialRects[2].w / 2, -ipDialRects[2].h / 2);
        SprPopTransform();
        sprState.col[0].a = sprState.col[0].a * 0.5f;
        SprPushTransform(0, -96.0f, 72.0f, ipGauge.highlight * ipGauge.highlight * 6.0f + 1.0f, ipGauge.highlight * ipGauge.highlight * 6.0f + 1.0f);
        SprDrawRect(&ipDialRects[2], -ipDialRects[2].w / 2, -ipDialRects[2].h / 2);
        SprPopTransform();
        sprState.col[0].a = 0x80;
    }
    sprState.offX = -0x54;
    sprState.offY = -0x38;
    SprDrawRect(&ipDialRects[3], -ipDialRects[3].w / 2, -ipDialRects[3].h / 2);
    SprFlush();
    sprState.offY = 0;
    sprState.offX = 0;
    GphTexBind(0xEA);

    lp = list;
    for (i = 0; i < 16; i++) {
        *lp++ = NULL;
    }

    start = 0;
    ch = btlUnits;
    bt = &ipGauge;
    for (i = 0; i < 4; i++, ch++) {
        if (ch->state < 0x40) {
            continue;
        }
        if (bt->actor >= 0) {
            if (i == bt->actor) {
                continue;
            }
            if (i == chr(bt->actor)->target) {
                continue;
            }
        }
        j = start;
        do {
            pp = &list[j];
            if (*pp != NULL) {
                if (ipGauge.icons[(*pp)->id].pos <= ipGauge.icons[ch->id].pos) {
                    continue;
                }
                for (k = 4 - 2; k >= j; k--) {
                    list[k + 1] = list[k];
                }
            }
            *pp = ch;
            break;
        } while (++j < 4);
    }
    if (ipGauge.actor >= 0) {
        if (ipGauge.actor != chr(ipGauge.actor)->target && chr(ipGauge.actor)->target < 4) {
            list[2] = chr(ipGauge.actor);
            list[3] = chr(chr(ipGauge.actor)->target);
        } else {
            list[3] = chr(ipGauge.actor);
        }
    }
    start2 = 4;
    for (; i < 16; i++, ch++) {
        if (ch->state < 0x40) {
            continue;
        }
        if (ch->action == 5) {
            continue;
        }
        if (ipGauge.actor >= 0) {
            if (i == chr(ipGauge.actor)->target) {
                continue;
            }
        }
        j2 = start2;
        do {
            pp2 = &list[j2];
            if (*pp2 != NULL) {
                if (ipGauge.icons[(*pp2)->id].pos <= ipGauge.icons[ch->id].pos) {
                    continue;
                }
                for (k = 16 - 2; k >= j2; k--) {
                    list[k + 1] = list[k];
                }
            }
            *pp2 = ch;
            break;
        } while (++j2 < 16);
    }
    if (ipGauge.actor >= 0) {
        if (chr(ipGauge.actor)->target >= 4) {
            list[15] = chr(chr(ipGauge.actor)->target);
        }
    }

    lp = list;
    for (i = 0; i < 16; i++, lp++) {
        if (*lp == NULL) {
            continue;
        }
        e = &ipGauge.icons[(*lp)->id];
        val = e->pos;
        if (ipGauge.areaMask != 0 && (*lp)->id == chr(ipGauge.actor)->target) {
            pulse = VuSinAngle(btlTickCount * 0x666) * 0.15f + 1.25f;
        } else {
            pulse = 1.0f;
        }
        alpha = 1.0f;
        switch ((*lp)->ipMode) {
        case 0:
            if (val < -12.5f) {
                val = -12.5f;
            }
            break;
        case 2:
            if (val < 50.0f) {
                val = 50.0f;
            }
            break;
        }
        sprState.col[0].a = alpha * 128.0f;
        if (i < 4) {
            angA = val * -655.36f + 32768.0f - 8192.0f;
            VuSinCosAngle(&a0, &a1, angA);
            a0 *= e->bounce + 32.0f;
            a1 *= e->bounce + 32.0f;
            if ((ipGauge.targetMask >> (*lp)->id) & 1) {
                col = VuSinAngle(btlTickCount << 12) * 48.0f + 207.0f;
                sprState.col[0].b = col;
                sprState.col[0].g = col;
                sprState.col[0].r = col;
            } else if ((ipGauge.areaMask >> (*lp)->id) & 1) {
                sprState.col[0].r = ABS(VuSinAngle(btlTickCount << 10)) * 120.0f + 128.0f;
                sprState.col[0].g = sprState.col[0].b = 128.0f - ABS(VuSinAngle(btlTickCount << 10)) * 48.0f;
            } else {
                sprState.col[0].b = 0x80;
                sprState.col[0].g = 0x80;
                sprState.col[0].r = 0x80;
            }
            SprPushTransform(0x8000 - angA, a0, a1, 1.0f, 1.0f);
            SprDrawRect(&ipPartyPointerRect, -8, -0x18);
            SprPopTransform();
            if (0.0f < e->spread) {
                VuSinCosAngle(&b0, &b1, 0x10000 - ((btlTickCount << 10) + e->spreadAngle));
                a0 += b0 * e->spread;
                a1 += b1 * e->spread;
            }
            if (e->flash > 0) {
                if (e->flash > 0x20) {
                    sz = VuSinAngle((e->flash - 0x20) << 10) * 0.5f + 1.0f;
                } else {
                    sz = 1.0f;
                }
                SprPushTransform(0, a0, a1, pulse * VuCosAngle((e->flash * e->flash) << 5) * sz, pulse * sz);
            } else {
                SprPushTransform(0, a0, a1, pulse, pulse);
            }
            ipPartyIconSprList[0].rect = &ipIconPartyRects[(*lp)->params->icon];
            ipPartyIconSprList[1].rect = &ipDialRects[9 - ((angA >> 9) & 3)];
            SprDrawRectList(ipPartyIconSprList);
            SprPopTransform();
        } else {
            angB = val * -655.36f + 32768.0f - 8192.0f;
            VuSinCosAngle(&c0, &c1, angB);
            c0 *= e->bounce + 80.0f;
            c1 *= e->bounce + 80.0f;
            if ((ipGauge.targetMask >> (*lp)->id) & 1) {
                col = VuSinAngle(btlTickCount << 12) * 48.0f + 207.0f;
                sprState.col[0].b = col;
                sprState.col[0].g = col;
                sprState.col[0].r = col;
            } else {
                sprState.col[0].b = 0x80;
                sprState.col[0].g = 0x80;
                sprState.col[0].r = 0x80;
            }
            SprPushTransform(-angB, c0, c1, 1.0f, 1.0f);
            SprDrawRect(&ipEnemyPointerRect, -5, -0x18);
            SprPopTransform();
            if (0.0f < e->spread) {
                VuSinCosAngle(&d0, &d1, 0x10000 - ((btlTickCount << 10) + e->spreadAngle));
                c0 += d0 * e->spread;
                c1 += d1 * e->spread;
            }
            if (e->flash > 0) {
                if (e->flash > 0x20) {
                    sz = VuSinAngle((e->flash - 0x20) << 10) * 0.5f + 1.0f;
                } else {
                    sz = 1.0f;
                }
                SprPushTransform(0, c0, c1, pulse * VuCosAngle((e->flash * e->flash) << 5) * sz, pulse * sz);
            } else {
                SprPushTransform(0, c0, c1, pulse, pulse);
            }
            ipEnemyIconSprList[0].rect = &menuHelpVsTypeIcons[(*lp)->params->icon];
            ipEnemyIconSprList[1].rect = &ipEnemyRingRects[(angB >> 9) & 3];
            SprDrawRectList(ipEnemyIconSprList);
            SprPopTransform();
            SprPushTransform(0, c0, c1, 1.0f, 1.0f);
            SprDrawRect(&ipIconUnitLabelRects[(*lp)->id], -0x16, -0x12);
            SprPopTransform();
        }
    }
    SprFlush();
    ipGauge.areaMask = 0;
    ipGauge.targetMask = 0;
    if (ipGauge.comboMark != 0) {
        GphTexBind(0x41);
        sprState.blend = 2;
        if ((btlTickCount & 0x7F) < 0x40) {
            sz = VuSinAngle(btlTickCount << 9);
            SprPushTransform(btlTickCount * 0x150, 4.0f, -6.0f, (VuSinAngle(btlTickCount << 11) + 1.0f) * sz, (VuCosAngle(btlTickCount << 11) + 1.0f) * sz);
            SprDrawRect(&ipComboMarkRect, -0x3D, -0x3D);
            SprPopTransform();
        }
        SprFlush();
    }
    fx = ipGauge.combo;
    if (fx->unit != 0) {
        fp = fx;
        GphTexBind(0x42);
        sprState.blend = 2;
        if (btlTickCount & 2) {
            SprSetColor(0xE6, 0x96, 0x41);
        } else {
            SprSetColor(0, 0x5A, 0xFA);
        }
        for (m = 0; m < 4 && fp->unit != 0; m++, fp++) {
            for (jj = 0; jj < 4; jj++) {
                t = (f32)(fp->timer - jj * 4) * 0.0625f;
                if (t < 0.0f) {
                    continue;
                }
                if (1.0f < t) {
                    continue;
                }
                if (t < 0.3f) {
                    sprState.col[0].a = t * 3.3333333f * 128.0f;
                } else {
                    sprState.col[0].a = 0x80;
                }
                scl = (f32)(4 - jj) + 4.0f;
                VuSinCosAngle(&e0, &e1, fp->ipPos * -655.36f + 32768.0f - 8192.0f);
                e0 *= 32.0f;
                e1 *= 32.0f;
                SprPushTransform(0, (e0 - 4.0f) * t + 4.0f, (e1 - -4.0f) * t + -4.0f, scl, scl);
                SprDrawRect(&ptclGlowRect, -1, -1);
                SprPopTransform();
            }
        }
        SprFlush();
        sprState.col[0].a = 0x80;
        sprState.col[0].b = 0x80;
        sprState.col[0].g = 0x80;
        sprState.col[0].r = 0x80;
    }
}
