#include "common.h"
#include "main/n03.h"

s32 McDirSizeKB(McDirList *arg0) {
    s32 sum = 0;
    s32 i;
    u32 *e;
    s32 r;

    if (arg0->count > 0) {
        e = &arg0->entries[0].FileSizeByte;
        for (i = 0; i < arg0->count; i++, e += 16) {
            if (*e != 0) {
                sum += (*e + 0x3FF) >> 10;
            }
        }
    }
    r = sum + 1;
    return r + ((arg0->count + 1) >> 1);
}
