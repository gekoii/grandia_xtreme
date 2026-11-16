#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccWipeForward(u8 *arg0) {
    OccWipe *a = OCC_PARAMS(arg0, OccWipe);

    if ((u16)a->param1 >= 2) {
        GphWipeStop();
        return;
    }
    GphWipeStart(a->type, a->param0, a->param1);
}
