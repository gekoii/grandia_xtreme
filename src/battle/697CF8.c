#include "common.h"
#include "battle/n29.h"

s32 FxAfterimageStop(void) {
    FxAfterimage *p = &fxAfterimage;
    if (p->unit == NULL) {
        return 1;
    }
    if (p->count <= 0) {
        return 0;
    }
    p->count = 0;
    return 0;
}
