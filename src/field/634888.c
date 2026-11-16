#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"
#include "grxsys.h"


s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void FldMenuMcDlgOkCancel(s32 arg0, s32 x, s32 y) {
    s32 font;
    FldMenuQuad *s;

    font = sysGrxsysPac->msg->tbl[11];
    s = FldMenuGetSprBuf(0xC);
    FldMenuWinSetLayer(0xB, 1, &fldMenuTextures[6], s, 128.0f);
    s->type = 1;
    s->v[0].r = 0x80;
    s->v[0].g = 0x80;
    s->v[0].b = 0x80;
    s->v[0].a = 0x80;
    s->v[0].x = x;
    s->v[0].y = y;
    s->v[1].x = x + 0x14;
    s->v[1].y = y + 0x14;
    s->z = 0;
    s->v[0].u = 0x56;
    s->v[0].v = 0xBE;
    s->v[1].u = 0x76;
    s->v[1].v = 0xDE;
    s++;
    s->type = 1;
    s->v[0].r = 0x80;
    s->v[0].g = 0x80;
    s->v[0].b = 0x80;
    s->v[0].a = 0x80;
    s->v[0].x = x + 0x5A;
    s->v[0].y = y;
    s->v[1].x = x + 0x6E;
    s->v[1].y = y + 0x14;
    s->z = 0;
    s->v[0].u = 0x56;
    s->v[0].v = 0xE0;
    s->v[1].u = 0x76;
    s->v[1].v = 0x100;
    s++;
    FldMenuWinTextAdd(0xB, 2, font, x + 0x18, y, 0x12, 0x12, 0x10, 0x13, 0);
    s->type = 0;
}
