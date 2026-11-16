#include "common.h"
#include "ptcl.h"

extern s32 ptclEmitterColorHeader;
void MemMove128(void *, s32 *, s32);

void PtclEmitterReset(u8 *arg0) {
    s32 i;
    s32 *a;
    s32 *b;

    if (arg0 != NULL) {
        a = (s32 *)&((PtclEmitter *)arg0)->verts[0].pos[3];
        b = (s32 *)((PtclEmitter *)arg0)->colorTag;
        for (i = 0; i < 128; i++) {
            a[i * 4] = 0x3F800000; /* 1.0f: w of every qword of verts[] */
            b[i] = 0x80000000;
        }
        MemMove128(((PtclEmitter *)arg0)->colorTag, &ptclEmitterColorHeader, 4);
    }
}
