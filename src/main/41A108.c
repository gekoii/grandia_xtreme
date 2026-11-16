#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

extern char menuHelpStatNames[][7];

s32 MenuHelpAddStats(s16 arg0, s16 arg1) {
    MenuHelp *m;
    PtyItem *o;
    PtyEquipData *cnt;
    MenuStatLine *e;
    s8 n;
    s32 x;

    m = &menuHelp;
    n = 0;
    o = m->data;
    cnt = o->equip;
    if (o->type == 9) {
        menuHelpStatLines[0].kind = 5;
        n = 1;
        if (cnt->stats[1] != 0) {
            menuHelpStatLines[1].kind = 1;
            n = 2;
        }
    } else if ((u8)(o->type - 6) < 3) {
        menuHelpStatLines[0].kind = 1;
        n = 1;
        if (cnt->stats[3] != 0) {
            n = 2;
            menuHelpStatLines[1].kind = 3;
        }
    } else if (o->type == 5) {
        menuHelpStatLines[0].kind = 0;
        n = 1;
        if (cnt->stats[1] != 0) {
            menuHelpStatLines[1].kind = 1;
            n = 2;
        }
    }
    if (m->style == 0 || o->type == 10) {
        if (o->type != 5 && n < 4 && cnt->stats[0] != 0) {
            menuHelpStatLines[n].kind = 0;
            n++;
        }
        if (o->type == 10 && n < 4 && cnt->stats[1] != 0) {
            menuHelpStatLines[n].kind = 1;
            n++;
        }
        if (n < 4 && cnt->stats[2] != 0) {
            menuHelpStatLines[n].kind = 2;
            n++;
        }
        if (!((u8)(o->type - 6) < 3) && n < 4 && cnt->stats[3] != 0) {
            menuHelpStatLines[n].kind = 3;
            n++;
        }
        if (n < 4 && cnt->stats[4] != 0) {
            menuHelpStatLines[n].kind = 4;
            n++;
        }
        if (o->type != 9 && n < 4 && cnt->stats[5] != 0) {
            menuHelpStatLines[n].kind = 5;
            n++;
        }
        if (n == 0) {
            return 0;
        }
    }
    while (n < 4) {
        menuHelpStatLines[n].kind = -1;
            n++;
    }
    e = menuHelpStatLines;
    for (n = 0; n < 4; n++, e++) {
        if (e->kind == -1) {
            continue;
        }
        switch (e->kind) {
        case 0:
            e->value = cnt->stats[0];
            break;
        case 1:
            e->value = cnt->stats[1];
            break;
        case 2:
            e->value = cnt->stats[2];
            break;
        case 3:
            e->value = cnt->stats[3];
            break;
        case 4:
            e->value = cnt->stats[4];
            break;
        case 5:
            e->value = cnt->stats[5];
            break;
        }
        x = arg0 - (u16)m->frame.ox;
        e->text.x = x;
        e->text.y = arg1 - m->frame.oy;
        if (m->style == 0) {
            e->text.x = n * 0x50 + x;
        } else {
            if (n & 1) {
                e->text.x = x + 100;
            }
            if (n & 2) {
                e->text.y += 0x18;
            }
        }
        TxtObjSetString(&e->text, menuHelpGlyphBuf, menuHelpStatNames[e->kind]);
    }
    return 1;
}
