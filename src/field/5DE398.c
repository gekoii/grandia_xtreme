#include "common.h"
#include "grxsys.h"
#include "ptcl.h"
#include "field/state.h"
#include "protos/field.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);
void GsTexSetupDraw(s32, s32, void *, s32, s32, s32);

s32 EffInit(void) {
    s32 i;

    effMapWork = FldMemAlloc(0x11C0);
    MemSet(effMapWork, 0, 0x11C0);
    GsTexSetupDraw(0, 0, effMapWork->drawEnvs[0], 0, 1, 0);
    GsTexSetupDraw(0, 0, effMapWork->drawEnvs[1], 1, 0, 0);
    GsTexSetupDraw(0, 0, effMapWork->drawEnvs[2], 2, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[0], sysGrxsysPac->tex4C[2], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[1], sysGrxsysPac->tex4C[3], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[2], sysGrxsysPac->tex4C[4], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[3], sysGrxsysPac->tex4C[5], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[4], sysGrxsysPac->tex4C[6], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[5], sysGrxsysPac->tex11C[4], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[6], sysGrxsysPac->texF8[1], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[7], sysGrxsysPac->tex158, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&effMapWork->texObjs[8], sysGrxsysPac->tex4C[1], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    for (i = 0; i < 16; i++) {
        effMapWork->pool16[i] = FldMemAlloc(0x12D0);
        MemSet(effMapWork->pool16[i], 0, 0x12D0);
    }
    for (i = 0; i < 4; i++) {
        effMapWork->pool5A[i] = FldMemAlloc(0x12D0);
        MemSet(effMapWork->pool5A[i], 0, 0x12D0);
        effMapWork->pool5A[i]->unk12CC = FldMemAlloc(0x40);
        effMapWork->poolE0[i] = FldMemAlloc(0x12D0);
        MemSet(effMapWork->poolE0[i], 0, 0x12D0);
        effMapWork->pickupPool[i] = FldMemAlloc(0x12D0);
        MemSet(effMapWork->pickupPool[i], 0, 0x12D0);
        effMapWork->pickupPool[i]->unk12CC = FldMemAlloc(0x40);
        effMapWork->spotPool[i] = FldMemAlloc(0x12D0);
        MemSet(effMapWork->spotPool[i], 0, 0x12D0);
        effMapWork->spotPool[i]->unk12CC = FldMemAlloc(0x40);
        effMapWork->charaPool[i] = FldMemAlloc(0x12D0);
        MemSet(effMapWork->charaPool[i], 0, 0x12D0);
        effMapWork->charaPool[i]->unk12CC = FldMemAlloc(0x40);
    }
    effMapWork->spotOpenPool[0] = FldMemAlloc(0x12D0);
    MemSet(effMapWork->spotOpenPool[0], 0, 0x12D0);
    effMapWork->spotOpenPool[0]->unk12CC = FldMemAlloc(0x40);
    return 0;
}
