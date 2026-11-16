#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

#define SET(x) ((s32 (*)(s32))McSpaceCheckSetPortState)(x)

void McSpaceCheckCardType(void) {
    McPortState *c;

    c = McGetCurPort();
    if (c->syncResult > 0) {
        if (c->state == 0) {
            goto set40B;
        }
        if (c->state != 1) {
            if (c->type != 2) {
            set40B:
                SET(0x40B);
            } else if (c->formatted == 0) {
                SET(0x40A);
            } else {
                SET(0x408);
            }
        }
    }
}
