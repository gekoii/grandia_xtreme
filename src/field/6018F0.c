#include "common.h"
#include "grxsys.h"
#include "field/n21.h"
#include "protos/field.h"


void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldThreatGaugeInit(void) {
    fldThreatGauge = FldMemAlloc(0xDC0);
    MemSet(fldThreatGauge, 0, 0xDC0);
    GsTexLoadPacket(fldThreatGauge->texPacket, sysGrxsysPac->tex48, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
}
