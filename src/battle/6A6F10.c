/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "party.h"
#include "battle/n30.h"

/* = ipGauge.icons / ipGauge.combo (this file only matches through the
 * separate symbols) */

void IpComboPreviewSet(PtyMove *arg0) {
    IpComboPreview *e;
    BtlUnit *u;
    s32 i;
    s32 j;
    s32 k;

    e = ipComboPreview;
    if (arg0 == NULL) {
        for (i = 0; i < 4; i++, e++) {
            e->unit = NULL;
        }
        return;
    }
    for (i = 0; i < 4 && arg0->members[i] > 0; i++, e++) {
        u = btlUnits;
        for (k = 0; k < 4; k++, u++) {
            if (u->params->id - 0x1000 == arg0->members[i]) {
                e->unit = u;
                e->ipPos = ipIcons[u->id].pos;
                switch (u->ipMode) {
                case 0:
                    if (e->ipPos < -12.5f) {
                        e->ipPos = -12.5f;
                    }
                    break;
                case 2:
                    if (e->ipPos < 50.0f) {
                        e->ipPos = 50.0f;
                    }
                    if (u->state != 0x81) {
                        e->ipPos -= 100.0f;
                    }
                    break;
                }
            }
        }
    }
    for (; i < 4; i++, e++) {
        e->unit = NULL;
    }
    e = ipComboPreview;
    for (i = 0; i < 4 && e->unit != NULL; i++, e++) {
        e->timer = 0x4C;
        for (j = 0; j < 4 && ipComboPreview[j].unit != NULL; j++) {
            if (j != i && e->ipPos < ipComboPreview[j].ipPos) {
                e->timer -= 12;
            }
        }
    }
}
