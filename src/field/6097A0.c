#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

s32 AmapCorridorWideRoll(u8 *p) {
    s32 r;
    u8 k;

    k = ((AmapWork *)p)->unk1D; /* p is the AmapWork (u8 * in the generated prototype) */
    r = 0;
    if (k != 1) {
        if (k == 2) {
            r = 1;
        } else if (VuRandInt(100) <= 0x31) {
            r = 1;
        }
    }
    return r;
}
