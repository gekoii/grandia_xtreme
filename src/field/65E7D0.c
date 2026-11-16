#include "common.h"
#include "protos/field.h"
#include "field/n26.h"


void McSaveCheckSpace(void) {
    McPortState *st;
    s32 n;
    s32 t;

    st = McGetCurPort();
    n = McSaveSizeKB();
    t = st->freeKB;
    if (t + McDirSizeKB(mcDirListPtr) >= n) {
        McSaveSetState(0x104);
        return;
    }
    McSaveSetState(0x300);
}
