#include "common.h"
#include "field/state.h"

void ChrStunStart(ChrObj *c, s32 n) {
    u8 st;

    if (n != 0) {
        st = c->stunState;
        c->stunTimer += n;
        if (st == 3 || st == 0) {
            c->stunState = 1;
        }
    }
}
