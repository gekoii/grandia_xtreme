/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n02.h"


void MenuTextDraw(TxtObj *p) {
    s32 ok;
    f32 sx;
    f32 sy;
    s32 flags;
    s32 r;
    s32 g;
    s32 b;
    s32 a;

    ok = 0;
    if (p->glyphLast < p->glyphFirst) {
        return;
    }
    if (p->sprites == NULL) {
        p->sprites = MemAlloc16A((p->glyphLast - p->glyphFirst + 2) * 8);
        p->rects = MemAlloc16A((p->glyphLast - p->glyphFirst + 1) * 12);
        if (p->sprites == NULL || p->rects == NULL) {
            ok = -1;
        } else {
            MenuTextLayout(p, (f32)p->x / p->scale - 1.0f, (f32)p->y / p->scale - 1.0f);
            ok = 1;
        }
    }
    if (ok >= 0) {
        sx = sprState.sx;
        r = (s8)sprState.col[0].r;
        g = (s8)sprState.col[0].g;
        b = (s8)sprState.col[0].b;
        sprState.col[0].r = menuTextAddR;
        sprState.col[0].g = menuTextAddG;
        sprState.col[0].b = menuTextAddB;
        sy = sprState.sy;
        flags = sprState.flags;
        sprState.flags = flags & 0xFFFFFFFB;
        sprState.sx = sx * p->scale;
        sprState.sy = sy * p->scale;
        if (p->style & TXT_STYLE_SKEW) {
            sprState.skewX += menuTextOffsetX;
            sprState.skewY += menuTextOffsetY;
        }
        if ((u16)sprState.angle == 0 && !(p->style & TXT_STYLE_SKEW)) {
            switch (p->style & 0xF) {
            case 1:
                sprState.offX += 2;
                sprState.offY += 2;
                SprDrawRectFastList(p->sprites);
                sprState.offX -= 2;
                sprState.offY -= 2;
                break;
            case 2:
                a = (s8)sprState.col[0].a;
                sprState.offY += 2;
                sprState.col[0].a = a * 0.6f;
                SprDrawRectFastList(p->sprites);
                sprState.offX -= 2;
                SprDrawRectFastList(p->sprites);
                sprState.offY -= 2;
                SprDrawRectFastList(p->sprites);
                sprState.offY -= 2;
                SprDrawRectFastList(p->sprites);
                sprState.offX += 2;
                SprDrawRectFastList(p->sprites);
                sprState.offX += 2;
                SprDrawRectFastList(p->sprites);
                sprState.offY += 2;
                SprDrawRectFastList(p->sprites);
                sprState.offY += 2;
                SprDrawRectFastList(p->sprites);
                sprState.col[0].a = a;
                sprState.offX -= 2;
                sprState.offY -= 2;
                break;
            }
        } else {
            switch (p->style & 0xF) {
            case 1:
                sprState.offX += 2;
                sprState.offY += 2;
                SprDrawRectList(p->sprites);
                sprState.offX -= 2;
                sprState.offY -= 2;
                break;
            case 2:
                a = (s8)sprState.col[0].a;
                sprState.offY += 2;
                sprState.col[0].a = a * 0.6f;
                SprDrawRectList(p->sprites);
                sprState.offX -= 2;
                SprDrawRectList(p->sprites);
                sprState.offY -= 2;
                SprDrawRectList(p->sprites);
                sprState.offY -= 2;
                SprDrawRectList(p->sprites);
                sprState.offX += 2;
                SprDrawRectList(p->sprites);
                sprState.offX += 2;
                SprDrawRectList(p->sprites);
                sprState.offY += 2;
                SprDrawRectList(p->sprites);
                sprState.offY += 2;
                SprDrawRectList(p->sprites);
                sprState.col[0].a = a;
                sprState.offX -= 2;
                sprState.offY -= 2;
                break;
            }
        }
        sprState.col[0].r = r;
        sprState.col[0].g = g;
        sprState.col[0].b = b;
        SprFlush();
        if (p->style & TXT_STYLE_GRADIENT) {
            sprState.col[3].a = sprState.col[0].a;
            sprState.col[2].a = sprState.col[0].a;
            sprState.col[1].a = sprState.col[0].a;
            sprState.col[3].r = menuTextGradR;
            sprState.flags |= 4;
            sprState.col[2].g = sprState.col[0].g;
            sprState.col[3].g = menuTextGradGValue;
            sprState.col[2].r = sprState.col[0].r;
            sprState.col[2].b = sprState.col[0].b;
            sprState.col[1].r = menuTextGradR;
            sprState.col[1].g = menuTextGradGValue;
            sprState.col[3].b = menuTextGradB;
            sprState.col[1].b = menuTextGradB;
            SprDrawRectList(p->sprites);
        } else if ((u16)sprState.angle == 0 && !(p->style & TXT_STYLE_SKEW)) {
            SprDrawRectFastList(p->sprites);
        } else {
            SprDrawRectList(p->sprites);
        }
        sprState.sx = sx;
        sprState.sy = sy;
        sprState.flags = flags;
        if (p->style & TXT_STYLE_SKEW) {
            sprState.skewX -= menuTextOffsetX;
            sprState.skewY -= menuTextOffsetY;
        }
    }
    menuTextGradB = 0x80;
    menuTextAddB = 0;
    menuTextGradGValue = 0x80;
    menuTextAddG = 0;
    menuTextGradR = 0x80;
    menuTextOffsetX = 4;
    menuTextAddR = 0;
    menuTextOffsetY = 0;
    if (ok != 0) {
        if (p->sprites != NULL) {
            MemFree(p->sprites);
        }
        if (p->rects != NULL) {
            MemFree(p->rects);
        }
        p->sprites = NULL;
    }
}
