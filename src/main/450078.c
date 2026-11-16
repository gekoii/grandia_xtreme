#include "common.h"
#include "protos/main.h"
#include "ptcl.h"

s32 MemMove128(void *, void *, s32);

void PtclSpriteSetAnimUvs(PtclGen *p) {
    s32 *cnt;
    s32 i;
    s32 j;
    GphAnimCell *e;
    PtclSpriteKey *c;

    cnt = p->groupLive;
    GphAnimSetSelect(p->animSet);
    for (i = 0; i < p->groupCount; i++, cnt++) {
        if (*cnt > 0) {
            c = ((PtclSpriteKey (*)[31])p->keys)[i];
            e = &((GphAnimCell (*)[32])p->uvPkt)[i][1];
            for (j = 0; j < 31; j++, c++, e++) {
                if (c->life != 0) {
                    MemMove128(e, GphAnimGetCellUv(((GphAnimState *)p->anims)[i * 31 + j].cell), 1);
                }
            }
        }
    }
}
