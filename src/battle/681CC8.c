#include "common.h"
#include "battle/n28.h"

void FxHomingSetState(FxHoming *p, s32 a, s32 b) {
    if (p != NULL) {
        p->state = a;
        p->unk4 = b;
    }
}
