#include "common.h"
#include "sys.h"
#include "protos/main.h"


typedef struct {
    u8 b[17];
} Name;

extern Name sysMovieFileTable[];

s32 SysMoviePlay(s32 arg0, s32 arg1) {
    s32 r;

    arg1 = SysIsDebugMode() ? arg1 : (arg0 ? 0 : arg1);
    MemHeapSelect(sysState.dataHeap);
    MemHeapReset(sysState.dataHeap);
    GsVsyncExit();
    r = MovPlay(&sysMovieFileTable[arg0], 0, 0, arg1, MemAlloc128(0xCDFF00));
    MemHeapReset(sysState.dataHeap);
    GphFbStoreImage(0, 0, 0x280, 0x1C0, sysState.screenCaptureAddr);
    SysReinitGraphics(1);
    return r;
}
