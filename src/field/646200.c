#include "common.h"
#include "field/state.h"
#include "field/n24.h"

void MiniConcertCarrosGet(ChrObj *out[5][5]) {
    u32 i, j;
    ChrObj *p;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            p = miniCtl->carros[i][j];
            out[i][j] = p;
            p->flags &= ~1;
        }
    }
}
