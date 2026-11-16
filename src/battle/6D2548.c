/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudDamageNumDraw(void) {
    BtlHudDamageNum *d;
    s32 i;
    s32 t;
    f32 f;
    f32 y;
    f32 g;
    f32 h;
    s32 n;

    GphTexBind(0xEB);
    SprBegin(0.0f, 0.0f);
    d = btlHudDamageNums;
    for (i = 0; i < 0x20; i++, d++) {
        if (d->active == 0 || d->timer < 0) {
            continue;
        }
        UtilWorldToScreen(&d->scr, &d->pos);
        sprState.ox = d->scr[0] + d->dx;
        sprState.oy = d->scr[1] + d->dy + d->stackY + 64.0f;
        if (d->flags & 0x100) {
            SprSetColor(0x70, 0x60, 0);
        } else {
            SprSetColor(0x80, 0x80, 0x80);
        }
        if (d->timer > 0x20) {
            sprState.col[0].a = (64.0f - d->timer) * 0.03125f * 128.0f;
        } else {
            sprState.col[0].a = 0x80;
        }
        if (d->flags & 0x20) {
            if (d->timer < 0x20) {
                SprSetScale(1.3f, d->scale);
            } else {
                f = d->scale + 0.3f;
                sprState.sy = f;
                sprState.sx = f;
            }
        } else {
            sprState.sx = sprState.sy = d->scale;
        }
        MenuNumberDraw(&btlHudDamageNumFmt, d->value);
    }
    SprFlush();
    GphTexBind(0xE9);
    SprSetColor(0x80, 0x80, 0x80);
    d = btlHudDamageNums;
    for (i = 0; i < 0x20; i++, d++) {
        if (d->active == 0 || (t = d->timer) < 0) {
            continue;
        }
        sprState.ox = d->scr[0] + d->dx;
        sprState.oy = d->scr[1] + d->dy + d->stackY + 64.0f;
        if (t > 0x20) {
            sprState.col[0].a = (64.0f - t) * 0.03125f * 128.0f;
        } else {
            sprState.col[0].a = 0x80;
        }
        if (d->flags & 0x20) {
            if (d->timer < 0x20) {
                SprSetScale(1.3f, d->scale);
            } else {
                f = d->scale + 0.3f;
                sprState.sy = f;
                sprState.sx = f;
            }
        } else {
            sprState.sx = sprState.sy = d->scale;
        }
        if (*(u64 *)d & 0xC000000000LL) {
            sprState.offX = MenuNumberWidth(&btlHudDamageNumFmt, d->value) * (1.0f - btlHudDamageNumFmt.sx) + btlHudDamageNumFmt.x;
            SprDrawRect((d->flags & 0x40) ? &btlHudMpSuffixRect : &btlHudSpSuffixRect, 0, 3);
            sprState.offX = 0;
        }
        sprState.ox = d->scr[0];
        if (d->flags & 2) {
            if (d->timer < 10) {
                g = d->timer * 0.1f;
                sprState.sy = g;
                sprState.sx = (1.0f - g) * (1.0f - g) * 4.0f + 1.0f;
            } else {
                sprState.sy = 1.0f;
                sprState.sx = 1.0f;
            }
            sprState.oy = d->scr[1] - 48.0f;
            SprDrawRectList(btlHudCriticalLabel);
        }
        if ((d->flags & 1) && d->timer >= 5) {
            if (d->timer < 0x18) {
                sprState.sy = 1.0f;
                h = (d->timer - 4) * 0.05f;
                sprState.sx = h * h;
            } else {
                sprState.sy = 1.0f;
                sprState.sx = 1.0f;
            }
            sprState.oy = d->scr[1];
            SprDrawRectList(btlHudCancelLabel);
        } else if (d->flags & 8) {
            n = d->timer;
            if ((f32)n < 48.0f) {
                if (n > 0x20) {
                    sprState.col[0].a = (48.0f - (f32)n) * 0.0625f * 128.0f;
                } else {
                    sprState.col[0].a = 0x80;
                }
                sprState.sx = 1.0f;
                sprState.sy = 1.0f;
                y = d->scr[1];
                sprState.oy = y;
                t = d->timer;
                if (t < 0x18 && (t & 1)) {
                    sprState.oy = y + (24.0f - (t & 0xFC)) * 0.041666668f * ((t & 2) ? 32.0f : -32.0f);
                }
                SprDrawRectList(btlHudCancelResistLabel);
            }
        }
    }
    SprFlush();
}
