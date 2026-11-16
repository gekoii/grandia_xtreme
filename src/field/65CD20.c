#include "common.h"
#include "protos/field.h"
#include "field/n26.h"


void McSpaceCheckFreeKBWithSaves(void) {
    McPortState *c;
    s32 n;
    s32 t;

    c = McGetCurPort();
    n = McSaveSizeKB();
    t = c->freeKB;
    if (t + McDirSizeKB(mcSpaceCheckDirList) >= n) {
        McSpaceCheckSetPortState(0x40A);
        return;
    }
    McSpaceCheckSetPortState(0x406);
}
