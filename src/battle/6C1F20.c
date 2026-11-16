/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/n32.h"

#define ROUNDUP(x) ((x) == (f32)(s32)(x) ? (x) : ((x) > 0.0f ? (f32)((s32)(x) + 1) : (f32)~(s32)(-x)))

s32 AiComboFindPartners(BtlUnit *arg0, BtlAction *g, s8 flag) {
    BtlUnit *u;
    s32 min;
    s32 i;

    min = ROUNDUP(g->level->cost * arg0->params->spcSpRate);
    aiComboMask = 1 << arg0->id;
    for (i = 1; i < 4; i++) {
        if (g->comboIds[i] > 0) {
            u = AiComboFindPartner(arg0, g->comboIds[i], min, flag);
            if (u == NULL) {
                return 0;
            }
            aiComboMask |= 1 << u->id;
        }
    }
    return 1;
}
