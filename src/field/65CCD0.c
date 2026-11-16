#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSpaceCheckFreeKB(void) {
    McPortState *c;

    c = McGetCurPort();
    if (c->freeKB >= McSaveSizeKB()) {
        McSpaceCheckSetPortState(0x40A);
        return;
    }
    McSpaceCheckSetPortState(0x402);
}
