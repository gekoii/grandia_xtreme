#include "common.h"
#include "gph.h"
GphAnimRect *GphAnimGetCellRect(u32 idx) {
    if (gphCurAnimSet == NULL) return NULL;
    if (idx >= gphCurAnimSet->cellCount) return NULL;
    return &gphCurAnimSet->cell.rects[idx];
}
