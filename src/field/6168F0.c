#include "common.h"

void AmapEmptyStub2(void) {

}

void AmapCellToPos(f32 *pos, s32 *x, s32 *z) {
    pos[0] = (*x + 1) * 500 + 250;
    pos[2] = -((*z - 1) * 500 + 250);
}
