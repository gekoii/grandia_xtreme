/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"


void ChrPartyFadeOutStep(void) {
    s32 i;
    ChrObj *c;
    f32 f;

    if (EvtSystemFlagGet(0xFA) == 1) {
        return;
    }
    if (MapLoadGetState() != 1) {
        return;
    }
    for (i = 0; i < 9; i++) {
        c = ChrGetByNo(i + 1);
        if (c->speed > 0.01f) {
            continue;
        }
        if (EvtSystemFlagGet(i + 0x1F) != 1 && i != 0) {
            continue;
        }
        f = ChrModelGetAlpha(c->model);
        ChrExtraPartsModeSet(c, 0);
        if (f > -127.0f) {
            ChrModelAddAlpha(c->model, -8.0f);
        } else if (c->flags & 8) {
            c->flags ^= 8;
        }
    }
}
