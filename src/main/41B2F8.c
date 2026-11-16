/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

f32 VuSinAngle(s32);

void MenuHelpUpdate(void) {
    MenuHelp *s = &menuHelp;
    f32 f;
    s8 n;
    f32 g;
    f32 t;

    if (s->state == 0) {
        return;
    }
    switch (s->state) {
    case 0x10:
        if (s->openFrame == menuHelpFrame) {
            break;
        }
        s->state = 0x20;
        s->frame.w = s->w;
        s->frame.bg.w = s->frame.w + 2;
        s->frame.h = s->h;
        s->frame.bg.h = s->frame.h + 2;
        s->frame.ox = s->w / 2;
        s->frame.bg.originX = s->frame.ox + 1;
        s->frame.oy = s->h / 2;
        s->frame.bg.originY = s->frame.oy + 1;
        TxtGlyphBufReset(menuHelpGlyphBuf);
        if (s->style == 2) {
            MenuHelpBuildTactics();
        } else {
            switch (s->kind) {
            case 0:
            case 1:
            case 2:
            case 3:
                MenuHelpBuildBasic();
                break;
            case 4:
                MenuHelpBuildWeapon();
                break;
            case 5:
                MenuHelpBuildArmor();
                break;
            }
        }
        s->alpha = 0.125f;
        if (s->style != s->nextStyle) {
            s->frame.alpha = 0.125f;
            g = VuSinAngle(0.125f * 16384.0f);
            s->frame.sy = g;
            s->frame.sx = 2.0f - g;
        } else {
            s->frame.alpha = 1.0f;
            s->frame.sy = 1.0f;
            s->frame.sx = 1.0f;
        }
        break;
    case 0x20:
        f = s->alpha + 0.25f;
        s->alpha = f;
        if (f >= 1.0f) {
            s->state++;
            s->frame.alpha = 1.0f;
            s->alpha = 1.0f;
            s->frame.sy = 1.0f;
            s->frame.sx = 1.0f;
            break;
        }
        if (s->style != s->nextStyle) {
            s->frame.alpha = f;
            g = VuSinAngle(f * 16384.0f);
            s->frame.sy = g;
            s->frame.sx = 2.0f - g;
        }
        break;
    case 0x21:
        break;
    case 0x30:
        f = s->alpha - 0.25f;
        s->alpha = f;
        if (f <= 0.0f) {
            if (s->nextStyle != 2 && s->nextData == NULL) {
                s->state = 0;
            } else {
                s->state = 0x10;
                s->openFrame = menuHelpFrame;
                if (s->style != s->nextStyle) {
                    s->style = s->nextStyle;
                    s->nextStyle = 0x7F;
                    s->slideTimer = 1;
                    s->frame.alpha = 0.0f;
                }
                s->data = s->nextData;
                s->kind = s->nextKind;
            }
            break;
        }
        if (s->style != s->nextStyle) {
            s->frame.alpha = f;
            g = VuSinAngle(f * 16384.0f);
            s->frame.sy = g;
            s->frame.sx = 2.0f - g;
        }
        break;
    }
    s->timer++;
    if (s->slideTimer > 0) {
        n = --s->slideTimer;
        if (n == 0) {
            s->frame.x = s->toX;
            s->frame.y = s->toY;
        } else {
            t = (VuSinAngle((n << 12) - 0x4000) + 1.0f) * 0.5f;
            s->frame.x = (s->fromX - s->toX) * t + s->toX;
            s->frame.y = (s->fromY - s->toY) * t + s->toY;
        }
    }
}
