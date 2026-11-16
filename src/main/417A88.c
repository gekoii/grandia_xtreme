/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

typedef struct Tex {
    u8 pad[8];
    s16 w;
    s16 padA;
    s16 h;
} Tex;

typedef struct Font {
    Tex *tex;
    u8 pad4[8];
    s8 cols;
    s8 padD;
    s8 padE;
    s8 cw;
    s8 ch;
} Font;


void MenuTextLayout(TxtObj *t, f32 x, f32 y) {
    s8 h1;
    s8 w1;
    s16 tw;
    s16 th;
    Tex *tex;
    Font *font;
    SprRect *uv;
    SprRectListEntry *spr;
    s8 col;
    s8 row;
    s32 n;
    f32 fy;
    f32 fx;

    font = t->glyphs;
    tex = font->tex;
    col = t->glyphFirst % font->cols;
    row = t->glyphFirst / font->cols;
    tw = tex->w;
    th = tex->h;
    uv = t->rects;
    uv->u0 = font->cw * col + 1;
    uv->v0 = t->glyphFirst / font->cols * font->ch + 1;
    uv->u1 = (u16)tw + uv->u0 + 1;
    uv->v1 = (u16)th + uv->v0 + 1;
    spr = t->sprites;
    fx = x - (f32)t->width / t->scale * t->alignX;
    fy = y - t->alignY * 24.0f;
    uv->w = 24;
    uv->h = 24;
    TxtFontImageGetGlyphMetrics(tex, col, row, &h1, &w1);
    fx -= w1 * 1.5f;
    spr->rect = uv;
    spr->y = fy;
    uv++;
    spr->x = fx;
    spr++;
    fx += h1 * 1.5f + t->spacing;
    for (n = t->glyphLast - t->glyphFirst; n > 0; n--) {
        uv->w = 24;
        uv->h = 24;
        col++;
        if (col >= font->cols) {
            uv->u0 = 1;
            row++;
            uv->u1 = tw + 1;
            col = 0;
            uv->v0 = (uv - 1)->v0 + font->ch;
            uv->v1 = (uv - 1)->v1 + font->ch;
        } else {
            uv->u0 = (uv - 1)->u0 + font->cw;
            uv->u1 = (uv - 1)->u1 + font->cw;
            uv->v0 = (uv - 1)->v0;
            uv->v1 = (uv - 1)->v1;
        }
        TxtFontImageGetGlyphMetrics(font->tex, col, row, &h1, &w1);
        fx -= w1 * 1.5f;
        spr->rect = uv;
        uv++;
        spr->y = fy;
        spr->x = fx;
        spr++;
        fx += h1 * 1.5f + t->spacing;
    }
    spr->rect = NULL;
}
