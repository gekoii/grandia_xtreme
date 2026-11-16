#include "common.h"
#include "field/menu.h"
#include "main/n03.h"


FldMenuQuad *FldMenuSaveFilePartyBuild(FldMenuQuad *s, McSlot *pt, s32 x, s32 y) {
    s32 i;
    s32 n;
    s32 id;

    McGetSlotTable();
    x += 0x14;
    n = 0;
    for (i = 0; i < 4; i++) {
        id = pt->party[i];
        if (id != 0) {
            s->type = 1;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0x80;
            s->v[0].x = x;
            s->v[0].y = y + 0x12;
            s->v[1].x = x + 0x36;
            s->v[1].y = y + 0x48;
            s->z = 0;
            id--;
            if (id < 4) {
                s->v[0].u = id * 0x36;
                s->v[0].v = 0;
                s->v[1].u = id * 0x36 + 0x36;
                s->v[1].v = 0x36;
            } else {
                s->v[0].u = (id - 4) * 0x36;
                s->v[0].v = 0x36;
                s->v[1].u = (id - 4) * 0x36 + 0x36;
                s->v[1].v = 0x6C;
            }
            n++;
            x += 0x78;
            s++;
            if (n >= 4) break;
        }
    }
    s->type = 0;
    return s;
}
