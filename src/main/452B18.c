#include "common.h"
#include "gph.h"




GphAnimCell *GphAnimGetCellUv(u32 arg0) {
    GphAnimSet *t = gphCurAnimSet;
    if (t == NULL) {
        return NULL;
    }
    if (arg0 >= t->cellCount) {
        return NULL;
    }
    return &t->cell.cells[arg0];
}
