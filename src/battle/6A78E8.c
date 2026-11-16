/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"

extern s32 btlTickCount;

f32 VuSinAngle(s32);

void IpGaugeUpdate(void) {
    IpGauge *p;
    IpComboPreview *e;
    BtlUnit *u;
    BtlUnit *o;
    s32 i;
    s32 k;
    s32 x;
    f32 v;
    f32 f;
    f32 t;
    f32 w;
    s32 h;

    p = &ipGauge;
    p->highlight -= 0.025f;
    if (p->highlight < 0.0f) {
        p->highlight = 0.0f;
    }
    if (p->shown == 0) {
        if (p->unk14 > 0) {
            if (--p->unk14 <= 0) {
                p->slide = 224.0f;
            } else {
                p->slide = (1.0f - VuSinAngle(p->unk14 << 10)) * 224.0f;
            }
        }
    } else {
        if (p->unk14 < 16) {
            if (++p->unk14 >= 16) {
                p->slide = 0.0f;
            } else {
                p->slide = (1.0f - VuSinAngle(p->unk14 << 10)) * 224.0f;
            }
        }
    }
    if (ipGauge.comboMark == 1) {
        if ((btlTickCount & 0x7F) == 0x18) {
            SndSePlay3D(0x7D7, 0, 0, 0);
        }
    } else if (ipGauge.comboMark == 2) {
        ipGauge.comboMark = 0;
    }
    if (ipTransDelay > 0) {
        if (--ipTransDelay == 0) {
            ipTransDelta = ipTransNextDelta;
        }
    }
    if (ipTransDelta != 0.0f) {
        ipTransRate += ipTransDelta;
        if (ipTransRate >= 1.0f) {
            ipTransDelta = 0.0f;
            ipTransRate = 1.0f;
        } else if (ipTransRate <= 0.0f) {
            ipTransDelta = 0.0f;
            ipTransRate = 0.0f;
        }
        t = 1.0f - (VuCosAngle(ipTransRate * 45056.0f - 6144.0f) * 1.20269f + 1.0f) * 0.5f;
        h = t * 24.0f;
        w = 1.0f - t * 0.3f;
        ipDialX = h;
        ipDialY = h;
        ipDialScaleX = ipDialScaleY = w;
    }
    u = btlUnits;
    for (i = 0; i < 16; i++, u++) {
        if (u->state >= 0x40 && u->comboPartners[0] >= 0) {
            v = ipIcons[u->id].pos;
            for (k = 0; k < 3 && u->comboPartners[k] >= 0; k++) {
                o = &btlUnits[u->comboPartners[k]];
                if (o->comboLeader >= 0 && o->action == 6) {
                    x = o->id * 0x18;
                    f = ((IpIcon *)(x + (u32)ipIcons))->pos;
                    if (f < v) {
                        v = f;
                    }
                }
            }
            ipIcons[u->id].pos = v;
            for (k = 0; k < 3 && u->comboPartners[k] >= 0; k++) {
                o = &btlUnits[u->comboPartners[k]];
                if (o->comboLeader >= 0 && o->action == 6) {
                    x = o->id * 0x18;
                    ((IpIcon *)(x + (u32)ipIcons))->pos = v;
                }
            }
        }
    }
    if (ipComboPreview[0].unit != NULL) {
        e = ipComboPreview;
        for (k = 0; k < 4 && e->unit != NULL; k++, e++) {
            e->timer++;
            if (e->timer >= 0x50) {
                e->timer = 0;
            }
        }
    }
}
