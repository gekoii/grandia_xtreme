#include "common.h"
#include "battle/env.h"
#include "battle/unit.h"

void EnvDimSetUnitLit(BtlUnit *u, s8 on) {
    EnvDim *r = &envDim;
    s32 i;
    s32 j;

    if (on) {
        r->mask |= 1 << u->id;
    } else {
        r->mask &= ~(1 << u->id);
    }
    if (u->comboPartners[0] >= 0) {
        for (i = 0; i < 3; i++) {
            if (u->comboPartners[i] >= 0) {
                if (on) {
                    r->mask |= 1 << u->comboPartners[i];
                } else {
                    r->mask &= ~(1 << u->comboPartners[i]);
                }
            }
        }
    }
    for (j = 0; j < 16; j++) {
        if ((r->mask >> j) & 1) {
            if (btlUnits[j].body >= 0) {
                if (on) {
                    r->mask |= 1 << btlUnits[j].body;
                } else {
                    r->mask &= ~(1 << btlUnits[j].body);
                }
            }
        }
    }
}
