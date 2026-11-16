#include "common.h"
#include "battle/n33.h"

void BtlHudDmgInit(void) {
    s32 i;
    BtlHudDamageNum *d;
    BtlHudUnitGauge *g;
    BtlHudComboPopup *p;
    BtlHudStatusIcon *e;
    BtlHudStatChange *b;
    BtlHudMarker *f;

    for (i = 0, d = btlHudDamageNums; i < 0x20; i++, d++) {
        d->active = 0;
    }
    for (i = 0, g = btlHudUnitGauges; i < 0x10; i++, g++) {
        g->hits = -1;
    }
    for (i = 0, p = btlHudEnemyComboPopups; i < 0xC; i++, p++) {
        p->rank = 0;
    }
    for (i = 0, p = btlHudPartyComboPopups; i < 0xC; i++, p++) {
        p->rank = 0;
    }
    for (i = 0, e = btlHudStatusIcons; i < 0x10; i++, e++) {
        e->flags = 0;
    }
    for (i = 0, b = btlHudStatChanges; i < 0x10; i++, b++) {
        b->powUp = 0;
    }
    btlHudMarkerNext = 0;
    for (i = 0, f = btlHudMarkers; i < 0x20; i++, f++) {
        f->type = 0;
    }
}
