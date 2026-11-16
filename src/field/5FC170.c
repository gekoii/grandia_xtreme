#include "common.h"
#include "field/event.h"
#include "field/n20.h"
#include "protos/field.h"

#define NEWLINE()                                                       \
    if (*p == 0x1F) {                                                   \
        p++;                                                            \
        cur->cursorX = 0;                                                     \
        cur->cursorY += 2;                                                    \
        if (arg1 != 0) {                                                \
            if ((EvtMsgGetCtl()->cursorY >> 1) == EvtMsgGetCtl()->choice) {    \
                EvtScnrSetTextColor(0xEF);                                    \
            } else {                                                    \
                EvtScnrSetTextColor(0xF);                                     \
            }                                                           \
        }                                                               \
    }

#define CTRL()                                                          \
    switch (*p) {                                                       \
    case 0x1A:                                                          \
        flags |= 8;                                                     \
        EvtMsgGetCtl()->flags |= 8;                                       \
        break;                                                          \
    case 0x17:                                                          \
        flags |= 8;                                                     \
        EvtMsgGetCtl()->flags |= 8;                                       \
        break;                                                          \
    case 0:                                                             \
        flags |= 8;                                                     \
        EvtMsgGetCtl()->flags |= 8;                                       \
        break;                                                          \
    default:                                                            \
        if (*p < 0x20) {                                                \
            flags |= 2;                                                 \
            EvtMsgGetCtl()->flags |= 2;                                   \
            if (arg1 == 0) {                                            \
                if (*p == 0xB || *p == 0x1C || *p == 0x1D || *p == 0x1E) { \
                    EvtMsgGetCtl()->flags |= 1;                           \
                }                                                       \
                if (*p == 5) {                                          \
                    EvtMsgGetCtl()->flags |= 1;                           \
                }                                                       \
                if (*p == 0x18) {                                       \
                    EvtMsgGetCtl()->flags |= 1;                           \
                }                                                       \
                if (*p == 0x1B) {                                       \
                    EvtMsgGetCtl()->flags |= 1;                           \
                }                                                       \
                if (*p == 0xD) {                                        \
                    EvtMsgGetCtl()->flags |= 1;                           \
                }                                                       \
            }                                                           \
        }                                                               \
        break;                                                          \
    }

u8 *EvtMsgLayoutRun(EvtMsgText *t, s32 arg1) {
    u16 ch;
    s32 h;
    s32 w;
    u8 *p;
    EvtMsgCtl *cur;
    u16 flags;

    cur = EvtMsgGetCtl();
    t->w = 0x1F8;
    t->h = 0x80;
    flags = cur->flags;
    flags &= 0xFFF5;
    p = t->text;
    if (EvtMsgGetCtl()->flags & 0x2000) {
        t->x = (p[-1] & 3) * 2;
    } else {
        t->x = 0;
    }
    t->y = 0;
    if (EvtMsgGetCtl()->flags & 0x20) {
        if ((EvtMsgGetCtl()->cursorY >> 1) == EvtMsgGetCtl()->choice) {
            EvtScnrSetTextColor(0xEF);
        } else {
            EvtScnrSetTextColor(0xF);
        }
    }
    do {
        NEWLINE();
        CTRL();
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
            EvtMsgGetCtl()->cursorX -= w;
        }
        TxtGlyphLayoutAddChar(t->layout, ch, EvtMsgGetCtl()->cursorX, EvtMsgGetCtl()->cursorY * 12);
        if (EvtMsgGlyphIsNarrow(ch) == 0) {
            if (w + h + 3 >= 0x1AU) {
                EvtMsgGetCtl()->cursorX += 0x18;
            } else {
                EvtMsgGetCtl()->cursorX += (s16)(w + h + 3);
            }
        } else {
            if (w + h + 3 >= 0xEU) {
                EvtMsgGetCtl()->cursorX += 0xE;
            } else {
                EvtMsgGetCtl()->cursorX += (s16)(w + h + 3);
            }
        }
        NEWLINE();
        CTRL();
    } while (!(flags & 0xA));
    EvtMsgGetCtl()->unk24 = 0;
    return p;
}
