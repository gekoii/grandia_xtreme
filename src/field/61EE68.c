#include "common.h"
#include "grxsys.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"

/* fldMenuRippleWork, fldMenuImageBuf and fldMenuParticleBuf are cleared and
   filled as one block (separate stores don't match). */
typedef struct {
    void *ripple;
    void *image;
    void *particle;
} Bufs;

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldMenuWorkInit(void) {
    FldMenuWork **pp;
    FldMenuWork *m;
    Bufs *b;

    pp = &fldMenuWork;
    m = FldMemAlloc(0xC6B0);
    *pp = m;
    MemSet(m, 0, 0xC6B0);
    b = (Bufs *)&fldMenuRippleWork;
    MemSet(b, 0, 0xC);
    GsTexLoadPacket(&fldMenuTextures[0], sysGrxsysPac->texE7[4], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[1], sysGrxsysPac->texF0, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[2], sysGrxsysPac->texE7[6], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[3], sysGrxsysPac->texEF, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[4], sysGrxsysPac->texF1, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[5], sysGrxsysPac->texE7[3], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[6], sysGrxsysPac->texE7[5], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[7], sysGrxsysPac->tex30, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexLoadPacket(&fldMenuTextures[8], sysGrxsysPac->texE7[2], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    fldMenuRippleWork = FldMemAlloc(GphRippleWorkSize(0x80, 0x80));
    b->image = FldMemAlloc(0x4000);
    b->particle = FldMemAlloc(0x780);
}
