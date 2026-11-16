#include "common.h"
#include "protos/main.h"

void GsVif1DmaWait(void);
void GsVif1DmaSend(void *, u32);
extern u32 gsVif0InitPacket[4];
extern u32 gsVif1InitPacket[56];

void GsVifInit(void) {
    GsVif0DmaSendN(gsVif0InitPacket, 1);
    GsVif0DmaWait();
    GsVif1DmaSend(gsVif1InitPacket, 4);
    GsVif1DmaWait();
}
