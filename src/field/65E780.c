#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveCheckSpaceNew(void) {
    McPortState *st;

    st = McGetCurPort();
    if (st->freeKB >= McSaveSizeKB()) {
        McSaveSetState(2);
        return;
    }
    McSaveSetState(0x300);
}
