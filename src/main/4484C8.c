#include "common.h"
#include "protos/main.h"


s32 FileChunkRelocate(s32 *p) {
    s32 tag;
    s32 size;
    s32 n;
    s32 *chunk;
    s32 *d;

    while ((tag = *p++, size = *p, p += 3, tag != 0)) {
        chunk = (s32 *)((u8 *)p + size);
        n = chunk[1];
        d = chunk + 4;
        if (chunk[0] == 0x30464F50) {
            FilePof0Apply(p, d, n);
            p = (s32 *)((u8 *)chunk + (n + 16));
        } else {
            p = chunk;
        }
    }
    return 0;
}
