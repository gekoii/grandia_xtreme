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

void MenuHelpAddName(s8 *arg0, s16 arg1, s16 arg2) {
    MenuHelp *c;
    Raw *r;
    s32 idx;
    s32 v;

    c = &menuHelp;
    idx = *arg0;
    v = 0;
    switch (c->kind) {
    case 0:
    case 1:
        v = ((PtyMove *)c->data)->help;
        break;
    case 2:
        v = ((PtySkillDef *)c->data)->help;
        break;
    case 3:
    case 4:
    case 5:
        v = ((PtyItem *)c->data)->help;
        break;
    }
    if (v != 0) {
        r = (Raw *)((u8 *)c + idx * 0x24);
        r->unk50 = 1;
        r->unk48 = arg1 - c->frame.ox;
        r->unk4A = arg2 - c->frame.oy;
        TxtObjSetString(&c->text[idx], menuHelpGlyphBuf, v);
        *arg0 = idx + 1;
    }
}
