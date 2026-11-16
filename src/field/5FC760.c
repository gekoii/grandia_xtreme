#include "common.h"
#include "field/event.h"
#include "field/n20.h"
#include "protos/field.h"

u8 *EvtMsgLayoutChars(EvtMsgText *t, s32 n) {
    u16 ch;
    s32 w;
    s32 h;
    s32 i;
    u8 *p;
    EvtMsgCtl *cur;
    s32 y;

    i = 0;
    cur = EvtMsgGetCtl();
    p = t->text;
    t->w = 0x18;
    t->h = 0x18;
    for (; i < n; i++) {
        if (*p == 0x1F) {
            p++;
            cur->cursorX = 0;
            cur->cursorY += 2;
        }
        if (p[1] >= 0xF0) {
            ch = p[1];
            ch = p[0] + (ch << 8);
            p += 2;
        } else {
            ch = p[0];
            p++;
        }
        EvtMsgGlyphMetrics(&ch, &w, &h);
        if (EvtMsgGetCtl()->cursorX != 0) {
            cur->cursorX -= w;
        }
        t->x = EvtMsgGetCtl()->cursorX;
        t->y = y = EvtMsgGetCtl()->cursorY;
        TxtGlyphLayoutAddChar(t->layout, ch, t->x, y * 12);
        if (EvtMsgGlyphIsNarrow(ch) == 0) {
            if (w + h + 3 >= 0x1AU) {
                cur->cursorX += 0x18;
            } else {
                cur->cursorX += (s16)(w + h + 3);
            }
        } else {
            if (w + h + 3 >= 0xEU) {
                cur->cursorX += 0xE;
            } else {
                cur->cursorX += (s16)(w + h + 3);
            }
        }
        if (*p == 0x1F) {
            p++;
            cur->cursorX = 0;
            cur->cursorY += 2;
        }
        if (*p < 0x20) {
            break;
        }
    }
    t->w = 0x1F8;
    t->h = 0x4A;
    t->x = 0;
    t->y = 0;
    return p;
}
