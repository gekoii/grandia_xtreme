#include "common.h"
#include "protos/main.h"


s32 FileChunk8Relocate(s32 *p, s32 len) {
    s32 *end;
    s32 tag;
    s32 size;
    s32 qsize;
    s32 *q;
    s32 *qd;

    end = (s32 *)((u8 *)p + len);
    while (p < end) {
        tag = *p++;
        size = *p++;
        if (tag == 0) {
            break;
        }
        q = (s32 *)((u8 *)p + size);
        qd = q + 2;
        qsize = q[1];
        if (q[0] == 0x30464F50) {
            FilePof0Apply(p, qd, qsize);
            p = (s32 *)((u8 *)q + (qsize + 8));
        } else {
            p = q;
        }
    }
    return 0;
}
