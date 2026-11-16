#include "common.h"

void GsVif1DmaWait(void);
void GsVif1DmaSend(void *, u32);
extern u32 gsVif1FlushPacket[4];
extern u32 gsVif1FlushEPacket[16];

void GsVif1Flush(void) {
    GsVif1DmaSend(gsVif1FlushPacket, 1);
    GsVif1DmaWait();
}

void GsVif1FlushE(void) {
    GsVif1DmaSend(gsVif1FlushEPacket, 1);
    GsVif1DmaWait();
}
