#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

/* menuHelp.text[i] addressed from the window base: (View *)((u8 *)&menuHelp + i * 0x24)
 * puts text[i].x/y/style at +0x48/+0x4A/+0x50. The match needs the 0x34 in
 * the field offsets rather than in the address. */
typedef struct {
    u8 pad0[0x48];
    s16 unk48;
    s16 unk4A;
    u8 pad4C[4];
    s8 unk50;
} Raw;

extern u8 menuHelpVsTypeNames[];
extern u8 menuHelpStrStrongVs[];
extern u8 menuHelpVsType;

void MenuHelpAddStrongVs(s8 *arg0, s16 arg1, s16 arg2) {
    MenuHelp *c;
    Raw *r;
    s32 idx;
    u8 v;

    c = &menuHelp;
    idx = *arg0;
    c->text[16].x = arg1 - c->frame.ox;
    c->text[16].y = arg2 - c->frame.oy;
    v = ((PtyItem *)c->data)->equip->unkE;
    menuHelpVsType = v;
    TxtObjSetString(&c->text[16], menuHelpGlyphBuf, menuHelpVsTypeNames + (s8)v * 7);
    r = (Raw *)((u8 *)c + idx * 0x24);
    r->unk50 = 1;
    r->unk48 = c->text[16].x + c->text[16].width;
    r->unk4A = c->text[16].y;
    TxtObjSetString(&c->text[idx], menuHelpGlyphBuf, menuHelpStrStrongVs);
    *arg0 = idx + 1;
}
