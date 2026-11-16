#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

#define SET(x) ((s32 (*)(s32))McSaveSetState)(x)

void McSaveCheckCard(void) {
    McPortState *c;

    c = McGetCurPort();
    if (c->state == 2) {
        if (c->type != c->state) {
            SET(0x30A);
        } else if (c->formatted == 0) {
            SET(0x305);
        } else {
            SET(0x101);
        }
    }
}
