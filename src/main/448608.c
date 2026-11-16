#include "common.h"
#include "main/n07.h"

s32 FileChunk8List(s32 *p, s32 size, FileChunkTable *d) {
    s32 *end = (s32 *)((u8 *)p + size);
    s32 n = 0;
    while (p < end) {
        s32 id = *p++;
        s32 len = *p++;
        if (id == 0) {
            break;
        }
        if (id != 0x30464F50) {
            d->entries[n].id = id;
            d->entries[n].data = p;
            n++;
        }
        p = (s32 *)((u8 *)p + len);
    }
    d->count = n;
    return 0;
}
