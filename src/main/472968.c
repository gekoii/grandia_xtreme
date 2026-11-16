/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0x70];
    s32 unk70;
    s32 pad74;
    s64 unk78;
} Inner;

typedef struct {
    u8 pad[0x40];
    Inner *unk40;
} Outer;

void sceMpegResetDefaultPtsGap(Outer *arg0) {
    Inner *p = arg0->unk40;
    p->unk70 = 0;
    p->unk78 = 0;
}
