/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "pad.h"
#include "party.h"
#include "field/state.h"
#include "field/n21.h"

void VuVecCopy(f32 *, f32 *);

void FldThreatGaugeUpdate(void) {
    ChrObj ent;
    GphModel mdl;
    s32 flags;
    s32 sum;
    s32 i;
    s32 j;
    ChrObj *e;
    FldEncGroup *r;
    u32 tot;
    u32 lv;
    u8 n;
    FldTreasure *base;
    FldTreasure *p;

    if (FldThreatGaugeIsHidden() == 1) {
        return;
    }
    flags = 0;
    sum = 0;
    for (i = 0; i < ChrPartyCount(); i++) {
        sum += ptyPartyUnits[i].level;
    }
    sum = sum / ChrPartyCount();
    for (i = 0; i < ChrTenantGetCount(); i++) {
        e = ChrTenantAt(i);
        if (!(e->flags & 0x10)) {
            continue;
        }
        if (!(e->flags & 0x1000)) {
            continue;
        }
        if (!(e->flags & 2)) {
            continue;
        }
        if (e->model == NULL) {
            continue;
        }
        if (!ChrTenantIsInLeaderView(e, 4000.0f, 180.0f)) {
            continue;
        }
        r = &fldEncGroups.groups[i];
        n = r->count;
        tot = 0;
        for (j = 0; j < n; j++) {
            tot += r->slots[j].level;
        }
        lv = tot / n;
        if (lv < sum) {
            flags |= 1;
        } else if (lv >= sum + 5) {
            flags |= 8;
        } else if (lv >= sum + 3) {
            flags |= 4;
        } else {
            flags |= 2;
        }
    }
    base = FldTreasureGetWork();
    if (base != NULL) {
        ent.radius = 40.0f;
        ent.flags = 1;
        ent.model = &mdl;
        for (i = 0; i < 64; i++) {
            p = &base[i];
            if (p->state < 2) {
                continue;
            }
            VuVecCopy(mdl.pos, p->path[3]);
            ChrTenantIsInLeaderView(&ent, 4000.0f, 180.0f);
        }
    }
    fldThreatGauge->seTimer++;
    if (fldThreatGauge->seTimer >= 30) {
        fldThreatGauge->seTimer = 0;
    }
    fldThreatGauge->alarm = 0;
    if (flags & 8) {
        fldThreatGauge->level = 19;
        fldThreatGauge->alarm = 1;
        fldThreatGauge->alarmFade += 0.005f;
        if (fldThreatGauge->alarmFade >= 1.0f) {
            fldThreatGauge->alarmFade = 1.0f;
        }
        if (fldThreatGauge->pulseSpeed < 8.0f) {
            fldThreatGauge->pulseSpeed += 0.05f;
        } else {
            fldThreatGauge->pulseSpeed -= 0.05f;
        }
        fldThreatGauge->vibrateTimer++;
        if (fldThreatGauge->vibrateTimer >= 20) {
            fldThreatGauge->vibrateTimer = 0;
            PadVibrate(padPort0, 1, 0x40, 4);
        }
    } else {
        fldThreatGauge->alarmFade -= 0.05f;
        if (fldThreatGauge->alarmFade <= 0.0f) {
            fldThreatGauge->alarmFade = 0.0f;
        }
        if (flags & 4) {
            fldThreatGauge->level = 9;
            if (fldThreatGauge->pulseSpeed < 5.0f) {
                fldThreatGauge->pulseSpeed += 0.05f;
            } else {
                fldThreatGauge->pulseSpeed -= 0.05f;
            }
        } else if (flags & 2) {
            fldThreatGauge->level = 6;
            if (fldThreatGauge->pulseSpeed < 3.0f) {
                fldThreatGauge->pulseSpeed += 0.05f;
            } else {
                fldThreatGauge->pulseSpeed -= 0.05f;
            }
        } else if (flags & 1) {
            fldThreatGauge->level = 4;
            if (fldThreatGauge->pulseSpeed < 2.0f) {
                fldThreatGauge->pulseSpeed += 0.05f;
            } else {
                fldThreatGauge->pulseSpeed -= 0.05f;
            }
        } else {
            fldThreatGauge->level = 0;
            fldThreatGauge->pulseSpeed -= 0.05f;
            if (fldThreatGauge->pulseSpeed <= 0.0f) {
                fldThreatGauge->pulseSpeed = 0.0f;
            }
        }
    }
    fldThreatGauge->pulseAngle += fldThreatGauge->pulseSpeed;
    if (fldThreatGauge->pulseAngle >= 360.0f) {
        fldThreatGauge->pulseAngle -= 360.0f;
    }
    fldThreatGauge->fadeIn += 0.05f;
    if (fldThreatGauge->fadeIn >= 1.0f) {
        fldThreatGauge->fadeIn = 1.0f;
    }
    if (fldThreatGauge->level != 0) {
        fldThreatGauge->level -= VuRandInt(2);
    }
    if (fldThreatGauge->level >= 19) {
        fldThreatGauge->level = 19;
    }
    for (i = 0; i < 19; i++) {
        if (i >= fldThreatGauge->level) {
            fldThreatGauge->barAlpha[i] -= 4;
            if (fldThreatGauge->barAlpha[i] < 0) {
                fldThreatGauge->barAlpha[i] = 0;
            }
        } else {
            if (i != 0) {
                if (fldThreatGauge->barAlpha[i - 1] >= 64) {
                    if (fldThreatGauge->alarm) {
                        fldThreatGauge->barAlpha[i] += i * 2 + 16;
                    } else {
                        fldThreatGauge->barAlpha[i] += 4;
                    }
                }
            } else {
                if (fldThreatGauge->alarm) {
                    fldThreatGauge->barAlpha[0] += 64;
                } else {
                    fldThreatGauge->barAlpha[0] += 4;
                }
            }
            if (fldThreatGauge->barAlpha[i] > 128) {
                fldThreatGauge->barAlpha[i] = 128;
            }
        }
    }
    if (flags != 0 && fldThreatGauge->prevFlags != flags) {
        SndSePlay(0x7F9, 0, 0, 30, 0);
    }
    fldThreatGauge->prevFlags = flags;
    if (fldThreatGauge->alarm != 0 && fldThreatGauge->seTimer == 0) {
        SndSePlay(0x7FA, 0, 0, 30, VuRandInt(256) & 0xFF);
    }
}
