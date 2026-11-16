/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0x70];
    s32 x70;
    s32 pad74;
    s64 x78;
} Inner72950;

typedef struct {
    u8 pad[0x40];
    Inner72950 *p40;
} Outer72950;

void sceMpegSetDefaultPtsGap(Outer72950 *arg0, s64 arg1) {
    Inner72950 *p = arg0->p40;
    p->x70 = 1;
    p->x78 = arg1;
}
