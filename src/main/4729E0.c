/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0xF0];
    s64 b;
    s32 a;
} S004729E0_in;

typedef struct {
    u8 pad[0x40];
    S004729E0_in *p;
} S004729E0;

void _sceMpegSetNextPts(S004729E0 *arg0, s64 arg1) {
    S004729E0_in *p = arg0->p;
    p->b = arg1;
    p->a = 1;
}
