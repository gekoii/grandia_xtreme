/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0x40];
    u8 *p;
} Obj;

void sceIpuStopDMA(void *);

void _sceMpegDefaultStopDMA(Obj *arg0) {
    sceIpuStopDMA(arg0->p + 0x4C);
}
