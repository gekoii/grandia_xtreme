#include "common.h"
#include "gph.h"

void GphAnimSetRelocate(GphAnimSet *arg0) {
    if (arg0 != NULL && arg0->cellCount != 0) {
        if (arg0->cell.cells != NULL) {
            arg0->cell.cells = (GphAnimCell *)((s32)arg0 + (s32)arg0->cell.cells);
        }
        if (arg0->keys != NULL) {
            arg0->keys = (GphAnimKey *)((s32)arg0 + (s32)arg0->keys);
        }
        if (arg0->tex != NULL) {
            arg0->tex = (void *)((s32)arg0 + (s32)arg0->tex);
        }
    }
}
