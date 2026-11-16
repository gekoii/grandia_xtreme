#include "common.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "sys.h"

void IpInit(void) {
    IpPanel *e;
    IpIcon *p;
    IpComboPreview *q;
    s32 i, j, k;

    btlHudHideFlags = 0x1000;
    ipGauge.highlight = 0.0f;
    ipGauge.actor = -1;
    ipGauge.slide = 224.0f;
    ipGauge.unk14 = ipGauge.shown = ipGauge.areaMask = ipGauge.targetMask = 0;
    ipGauge.comboMark = 0;
    if (btlHandoffIpStopOpt == 0) {
        ipTransRate = 1.0f;
        ipTransDelta = 1.0f;
    } else {
        ipTransRate = 0.0f;
        ipTransDelta = -1.0f;
    }
    ipTransDelay = 0;
    ipTransNextDelta = 0.0f;
    p = ipGauge.icons;
    for (j = 0xF; j >= 0; j--) {
        p->bounce = 0;
        p->flash = 0;
        p->spread = 0;
        p->spreadAngle = 0;
        p++;
    }
    q = ipGauge.combo;
    for (k = 3; k >= 0; k--) {
        q->unit = 0;
        q++;
    }
    e = ipPanels;
    for (i = 0; i < 4; i++) {
        e->mode = 1;
        e->unk4 = 1;
        e->pos.x = i * 160.0f + 8.0f;
        e->pos.y = -80.0f;
        e->shake.x = e->shake.y = 0.0f;
        e->sp = 0;
        e->icons[11].state = 0;
        e->icons[10].state = 0;
        e->icons[9].state = 0;
        e->icons[8].state = 0;
        e->icons[7].state = 0;
        e->icons[6].state = 0;
        e->icons[5].state = 0;
        e->icons[4].state = 0;
        e->icons[3].state = 0;
        e->icons[2].state = 0;
        e->icons[1].state = 0;
        e->icons[0].state = 0;
        e->flash = 0;
        e++;
    }
}
