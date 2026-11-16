#include "common.h"
#include "sys.h"
#include "protos/main.h"




void FlushCache(s32);

void SysProgLoad(u8 arg0) {
    sysState.program = arg0;
    FileGetSizeAligned(&sysProgramTable[sysState.program]);
    FileRead(&sysProgramTable[sysState.program], sysProgramTable[sysState.program].entry);
    FlushCache(0);
    FlushCache(2);
    SysProgCall(0, 0, 0);
}
