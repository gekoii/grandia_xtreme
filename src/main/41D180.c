#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "m2c_macros.h"


void SysTexturesReset(void) {
    GphTexTableInit(0x100, (s32)gphTexTableBuf);
    GphTexTableSelect(gphTexTableBuf);
    SysTexturesBind();
}
