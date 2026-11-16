#include "common.h"
#include "grxsys.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n25.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldSpriteInit(void) {
    GphAnimSet *r;

    ptclFldSlots = FldMemAlloc(0xC00);
    fldSpriteWork = FldMemAlloc(0x27C0);
    MemSet(ptclFldSlots, 0, 0xC00);
    MemSet(fldSpriteWork, 0, 0x27C0);
    r = sysGrxsysPac->animSets[0];
    GsTexLoadPacket(fldSpriteWork->texPackets[0], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSets[1];
    GsTexLoadPacket(fldSpriteWork->texPackets[1], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSets[2];
    GsTexLoadPacket(fldSpriteWork->texPackets[2], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSets[3];
    GsTexLoadPacket(fldSpriteWork->texPackets[3], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSets[4];
    GsTexLoadPacket(fldSpriteWork->texPackets[4], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSets[5];
    GsTexLoadPacket(fldSpriteWork->texPackets[5], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSet34;
    GsTexLoadPacket(fldSpriteWork->texPackets[6], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSets[4];
    GsTexLoadPacket(fldSpriteWork->texPackets[7], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    r = sysGrxsysPac->animSets[4];
    GsTexLoadPacket(fldSpriteWork->texPackets[8], r->tex, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
}
