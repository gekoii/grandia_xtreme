#include "common.h"
#include "protos/main.h"
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
    u8 pad51;
    s16 w;
} Ent41A;

extern u8 menuHelpEffectLabels[][31];

void MenuHelpAddEffectLabel(s8 *arg0, s8 arg1, s16 arg2, s16 arg3) {
    s8 i;
    Ent41A *e;
    Ent41A *p;
    u8 *base = (u8 *)&menuHelp;
    s8 sel;

    sel = arg1 - 5;
    i = *arg0;
    ((Ent41A *)(i * 0x24 + base))->flag = 1;
    ((Ent41A *)(i * 0x24 + base))->x = arg2 - ((MenuHelp *)base)->frame.ox;
    ((Ent41A *)(i * 0x24 + base))->y = arg3 - ((MenuHelp *)base)->frame.oy;
    switch (sel) {
    case 1:
        arg1 = 3;
        break;
    case 2:
        arg1 = 4;
        break;
    case 3:
        arg1 = 5;
        break;
    case 0:
        arg1 = 1;
        break;
    case 4:
        arg1 = 2;
        break;
    }
    TxtObjSetString(&base[i * 0x24 + 0x34], menuHelpGlyphBuf, menuHelpEffectLabels[arg1]);
    i++;
    e = (Ent41A *)(base + i * 0x24);
    e->flag = 1;
    p = (Ent41A *)((i - 1) * 0x24 + base);
    e->x = p->x + p->w;
    e->y = p->y;
    TxtObjSetString(&base[i * 0x24 + 0x34], menuHelpGlyphBuf, menuHelpEffectLabels);
    *arg0 = i + 1;
}
