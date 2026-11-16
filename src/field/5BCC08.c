#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccPlayerLadderForward(s32 arg0) {
    OccLadder *s;

    s = OCC_PARAMS(arg0, OccLadder);
    if (ChrGetByNo(1)->ctrlMode != 2) {
        ChrLadderStart(s->end0, s->unk18);
    }
}
