#include "common.h"
#include "battle/n29.h"

void FxAfterimageSetSpread(f32 fparg0) {
    if (fxAfterimage.unit != NULL && fxAfterimage.count > 0) {
        fxAfterimage.spread = fparg0;
    }
}
