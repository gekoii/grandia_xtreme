#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n29.h"
#include "battle/n30.h"
#include "party.h"
#include "sys.h"


extern SprRect ipPanelCharRects[];
extern SprRect ipPanelFrameRect;
extern u8 ipPanelHpNumFmt[];

void IpPanelDraw(void) {
    GphBatchQuad q[3];
    GphBatchQuad *p;
    IpPanel *e;
    BtlUnit *u;
    s32 i;
    s32 j;
    f32 fu;
    f32 fx;
    f32 fy;

    GphTexBind(0xEF);
    q[0].type = 1;
    q[0].v[0].r = 0x10;
    q[0].v[0].g = 0x10;
    q[0].v[0].b = 0x10;
    q[0].v[0].a = 0x40;
    q[0].v[1].r = 0x10;
    q[0].v[1].g = 0x10;
    q[0].v[1].b = 0x10;
    q[0].v[1].a = 0x40;
    q[0].v[2].r = 0x10;
    q[0].v[2].g = 0x10;
    q[0].v[2].b = 0x10;
    q[0].v[2].a = 0x40;
    q[0].v[3].r = 0x10;
    q[0].v[3].g = 0x10;
    q[0].v[3].b = 0x10;
    q[0].v[3].a = 0x40;
    q[1].type = 1;
    q[2].type = 0;
    for (i = 0, e = ipPanels, u = btlUnits; i < 4; i++, e++, u++) {
        if (u->state >= 2 && e->mode >= 0x10) {
            q[1].v[0].r = e->spGlow;
            q[1].v[0].g = e->spGlow;
            q[1].v[0].b = e->spGlow;
            q[1].v[0].a = 0x60;
            q[1].v[1].r = e->spGlow;
            q[1].v[1].g = e->spGlow;
            q[1].v[1].b = e->spGlow;
            q[1].v[1].a = 0x60;
            q[1].v[2].r = e->spGlow;
            q[1].v[2].g = e->spGlow;
            q[1].v[2].b = e->spGlow;
            q[1].v[2].a = 0x60;
            q[1].v[3].r = e->spGlow;
            q[1].v[3].g = e->spGlow;
            q[1].v[3].b = e->spGlow;
            q[1].v[3].a = 0x60;
            fu = 0x80 - (btlTickCount & 0x7F);
            p = q;
            q[0].v[0].u = q[0].v[1].u = fu;
            q[0].v[2].u = q[0].v[3].u = fu + 64.0f;
            q[0].v[0].v = q[0].v[2].v = 0x16;
            q[0].v[1].v = q[0].v[3].v = 0x11;
            fx = e->pos.x + e->shake.x + 39.0f;
            fy = e->pos.y + e->shake.y + 52.0f;
            q[0].v[0].x = q[0].v[1].x = fx;
            q[0].v[2].x = q[0].v[3].x = fx + e->spMax;
            q[0].v[0].y = q[0].v[2].y = fy;
            q[0].v[1].y = q[0].v[3].y = q[0].v[0].y - 12.0f;
            q[0].v[0].x -= 4;
            q[0].v[2].x -= 4;
            p++;
            p->v[0].u = p->v[1].u = fu;
            p->v[2].u = p->v[3].u = fu + e->sp * 0.64f;
            p->v[0].v = p->v[2].v = 0x16;
            p->v[1].v = p->v[3].v = 0x11;
            p->v[0].x = p->v[1].x = fx;
            p->v[2].x = p->v[3].x = p->v[0].x + e->sp;
            p->v[0].y = p->v[2].y = (p - 1)->v[0].y;
            p->v[1].y = p->v[3].y = (p - 1)->v[1].y;
            p->v[0].x -= 4;
            p->v[2].x -= 4;
            GphBatchDrawQuads(q);
        }
    }

    GphTexBind(0xEC);
    SprBegin(0.0f, 0.0f);
    for (i = 0, e = ipPanels, u = btlUnits; i < 4; i++, e++, u++) {
        if (u->state >= 2 && e->mode >= 0x10) {
            sprState.ox = e->pos.x + e->shake.x;
            sprState.oy = e->pos.y + e->shake.y;
            SprDrawRect(&ipPanelCharRects[u->params->id], 2, 2);
        }
    }
    SprFlush();

    GphTexBind(0xED);
    SprBegin(0.0f, 0.0f);
    for (i = 0, e = ipPanels, u = btlUnits; i < 4; i++, e++, u++) {
        if (u->state >= 2 && e->mode >= 0x10) {
            sprState.ox = e->pos.x + e->shake.x;
            sprState.oy = e->pos.y + e->shake.y;
            SprDrawRect(&ipPanelFrameRect, 0, 0);
        }
    }
    SprFlush();

    SprBegin(0.0f, 0.0f);
    for (i = 0, e = ipPanels, u = btlUnits; i < 4; i++, e++, u++) {
        if (u->state >= 2 && e->mode >= 0x10) {
            sprState.ox = e->pos.x + e->shake.x;
            sprState.oy = e->pos.y + e->shake.y;
            SprDrawRect(&btlStatLabelSprites[0], 0x3E, 1);
            SprSetColor(e->spGlow, e->spGlow, e->spGlow);
            SprDrawRect(&btlStatLabelSprites[2], 0x2B, 0x1D);
            SprSetColor(0x80, 0x80, 0x80);
        }
    }
    SprFlush();

    GphTexBind(0xEB);
    SprBegin(0.0f, 0.0f);
    for (i = 0, e = ipPanels, u = btlUnits; i < 4; i++, e++, u++) {
        if (u->state >= 2 && e->mode >= 0x10) {
            sprState.ox = e->pos.x + e->shake.x;
            sprState.oy = e->pos.y + e->shake.y;
            BtlHudSetHpColor(u->params->hp, u->params->maxHp);
            MenuNumberDraw(ipPanelHpNumFmt, u->params->hp);
        }
    }
    SprFlush();

    GphTexBind(0xEC);
    SprBegin(0.0f, 0.0f);
    for (i = 0, e = ipPanels, u = btlUnits; i < 4; i++, e++, u++) {
        if (u->state >= 2 && e->mode >= 0x10) {
            sprState.oy = e->pos.y + e->shake.y + 72.0f;
            for (j = 0; j < 12; j++) {
                if (e->icons[j].state != 0) {
                    sprState.sx = sprState.sy = e->icons[j].scale;
                    sprState.col[0].a = e->icons[j].alpha * 128.0f;
                    sprState.ox = e->pos.x + e->shake.x + 12.0f + e->icons[j].pos;
                    SprDrawRect(&menuStatusIcons[j], -12, -24);
                }
            }
        }
    }
    SprFlush();
}
