#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

/* menuHelp.text[i] addressed from the window base: (View *)((u8 *)&menuHelp + i * 0x24)
 * puts text[i].x/y/style at +0x48/+0x4A/+0x50. The match needs the 0x34 in
 * the field offsets rather than in the address. */
typedef struct {
    u8 pad0[0x48];
    s16 x;
    s16 y;
    u8 pad4C[4];
    s8 flag;
} View;

extern u8 menuHelpStrBreak[];
extern u8 menuHelpLinkValue;

void MenuHelpAddLinkedSkill(s8 *arg0, PtyItemUse *arg1, s16 arg2, s16 arg3) {
    s8 idx;
    s32 off;
    s32 off2;
    View *v;
    View *w;
    MenuHelp *b;
    PtyMove *ent;

    b = &menuHelp;
    idx = *arg0;
    ent = &ptyMoveTable[arg1->moveId];
    off = idx * 0x24;
    v = (View *)(off + (u8 *)b);
    v->flag = 1;
    v->x = arg2 - b->frame.ox;
    v->y = arg3 - b->frame.oy;
    TxtObjSetString(off + 0x34 + (u8 *)b, menuHelpGlyphBuf, (void *)ent->name);
    idx++;
    off2 = idx * 0x24;
    w = (View *)(off2 + (u8 *)b);
    w->flag = 2;
    w->x = 0x10A - b->frame.ox;
    w->y = arg3 - b->frame.oy;
    TxtObjSetString(off2 + 0x34 + (u8 *)b, menuHelpGlyphBuf, menuHelpStrBreak);
    menuHelpLinkValue = arg1->rate;
    menuHelpLinkNumField.x = w->x + 0x60;
    menuHelpLinkNumField.y = w->y - 3;
    *arg0 = idx + 1;
}
