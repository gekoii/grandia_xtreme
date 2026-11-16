#include "common.h"
#include "protos/field.h"
#include "field/occ.h"
#include "field/state.h"

void OccStateSetForwardStart(OccEntry *obj)
{
    OccStateSet *s = &obj->params.stateSet;
    ChrObj *e;

    obj->timer = 0;
    switch (s->op) {
    case 7:
        FldLightPresetChange(s->unk6, s->unk4);
        break;
    case 0:
        if (s->type >= 4 && s->type <= 6) {
            EvtFlagSetByType(s->type, s->id, s->value);
        }
        break;
    case 1:
        if (s->value != 0) {
            FldInputLockSet(1);
        } else {
            FldInputLockSet(0);
        }
        if (obj->flags & 0x100) {
            EvtSystemFlagSet(0x71, (u8)s->value);
        }
        e = ChrGetByNo(1);
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(e->vel) : "memory");
        break;
    }
}
