/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "party.h"

f32 VuSinAngle(s32);

void IpPanelUpdate(void) {
    IpPanel *p;
    PtyUnit *u;
    s32 i;
    s32 k;
    s32 mask;
    s32 n;
    s8 fl[6];
    f32 h;
    f32 y;

    p = ipPanels;
    for (i = 0; i < 4; i++, p++) {
        if ((btlUnits + i)->state < 2) {
            continue;
        }
        switch (p->mode) {
        case 0:
            if (p->timer == 0) {
                switch (i) {
                case 0:
                    p->pos.x = -160.0f;
                    p->pos.y = 12.0f;
                    break;
                case 1:
                case 2:
                    p->pos.y = -80.0f;
                    p->pos.x = i * 160.0f + 8.0f;
                    break;
                case 3:
                    p->pos.x = 640.0f;
                    p->pos.y = 12.0f;
                    break;
                }
                p->timer = 1;
            }
            break;
        case 1:
            if (p->timer == 0) {
                p->timer = 1;
                p->pos.y = -80.0f;
                p->pos.x = i * 160.0f + 8.0f;
            }
            break;
        case 16:
            if (p->timer < 16) {
                p->timer++;
                p->pos.x = p->from.x + ((i * 160.0f + 8.0f) - p->from.x) * (p->timer * 0.0625f);
                p->pos.y = p->from.y + (12.0f - p->from.y) * (p->timer * 0.0625f);
            } else if (p->timer == 16) {
                p->timer = 17;
                p->pos.y = 12.0f;
                p->pos.x = i * 160.0f + 8.0f;
            }
            break;
        case 17:
            if (p->timer < 16) {
                p->timer++;
                p->pos.x = p->from.x + (136.0f - p->from.x) * (p->timer * 0.0625f);
                p->pos.y = p->from.y + (12.0f - p->from.y) * (p->timer * 0.0625f);
            } else if (p->timer == 16) {
                p->pos.x = 136.0f;
                p->pos.y = 12.0f;
                p->timer = 17;
            }
            break;
        case 18:
            if ((btlUnits + i)->flags & 0x40) {
                p->pos.y -= p->timer;
                if (p->pos.y < -80.0f) {
                    p->mode = 0;
                }
            } else {
                switch (i) {
                case 0:
                    p->pos.x -= p->timer;
                    if (p->pos.x < -160.0f) {
                        p->mode = 0;
                    }
                    break;
                case 1:
                case 2:
                    p->pos.y -= p->timer;
                    if (p->pos.y < -80.0f) {
                        p->mode = 0;
                    }
                    break;
                case 3:
                    p->pos.x += p->timer;
                    if (p->pos.x > 640.0f) {
                        p->mode = 0;
                    }
                    break;
                }
            }
            p->timer++;
            break;
        }
        u = (btlUnits + i)->params;
        if (u->maxSp > 0) {
            if (u->sp < p->sp && p->sp > 0) {
                p->sp--;
            } else if (p->sp < u->sp && p->sp < p->spMax) {
                p->sp++;
            }
            if (u->sp >= u->maxSp) {
                p->spGlow = (VuSinAngle(btlTickCount << 12) + 1.0f) * 60.0f + 128.0f;
            } else {
                p->spGlow = 0x80;
            }
        }
        mask = (btlUnits + i)->params->status;
        for (k = 0; k < 12; k++) {
            IpPanelStatusIconSetActive(&p->icons[k], mask & (0x10 << k));
        }
        fl[0] = p->icons[0].state > 0 || p->icons[7].state > 0 || p->icons[10].state > 0 || p->icons[11].state > 0;
        fl[1] = p->icons[1].state > 0 || p->icons[2].state > 0 || p->icons[3].state > 0;
        fl[2] = p->icons[4].state > 0;
        fl[3] = p->icons[5].state > 0;
        fl[4] = p->icons[6].state > 0;
        fl[5] = p->icons[8].state > 0;
        n = fl[0] + fl[1] + fl[2] + fl[3] + fl[4] + fl[5];
        h = 3.20000076f;
        h *= 1.0f - (6 - n) / 6.0f;
        h = 24.0f - h;
        y = 0.0f;
        IpPanelStatusIconMove(&p->icons[0], y);
        IpPanelStatusIconMove(&p->icons[7], y);
        IpPanelStatusIconMove(&p->icons[10], y);
        IpPanelStatusIconMove(&p->icons[11], y);
        y += fl[0] ? h : 0.0f;
        IpPanelStatusIconMove(&p->icons[1], y);
        IpPanelStatusIconMove(&p->icons[2], y);
        IpPanelStatusIconMove(&p->icons[3], y);
        y += fl[1] ? h : 0.0f;
        IpPanelStatusIconMove(&p->icons[4], y);
        y += fl[2] ? h : 0.0f;
        IpPanelStatusIconMove(&p->icons[5], y);
        y += fl[3] ? h : 0.0f;
        IpPanelStatusIconMove(&p->icons[6], y);
        y += fl[4] ? h : 0.0f;
        IpPanelStatusIconMove(&p->icons[8], y);
        if (p->flash > 0) {
            if (!(p->flash & 1)) {
                p->shake.x = ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * p->flash * 0.625f;
                p->shake.y = p->flash * ((p->flash & 2) ? -0.25f : 0.25f);
            }
            if (--p->flash <= 0) {
                p->shake.x = p->shake.y = 0.0f;
            }
        }
    }
}
