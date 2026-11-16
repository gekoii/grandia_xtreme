#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

s32 FileChunkParse(s32 *buf, s32 len, FileChunkTable *tbl) {
    s32 *end;
    s32 n;
    s32 id;
    s32 size;
    s32 *q;
    s32 relsize;
    u8 *rel;

    end = (s32 *)((u8 *)buf + len);
    n = 0;
    while (buf < end) {
        id = *buf++;
        size = *buf;
        buf += 3;
        if (id == 0) {
            break;
        }
        q = (s32 *)((u8 *)buf + size);
        rel = (u8 *)(q + 2);
        tbl->entries[n].id = id;
        tbl->entries[n].data = buf;
        tbl->entries[n].size = size;
        n++;
        relsize = q[1];
        if (q[0] == 0x30464F50) {
            FilePof0Apply(buf, rel, relsize);
            buf = (s32 *)((u8 *)q + (relsize + 16));
        } else {
            buf = q;
        }
    }
    tbl->count = n;
    return 0;
}
