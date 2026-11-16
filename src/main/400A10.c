#include "common.h"
#include "sys.h"

typedef s32 (*EntryFn)(s32, s32);




s32 SysProgCall(s32 arg0, s32 arg1, s32 arg2) {
    EntryFn fn = NULL;
    s32 ret = -1;

    if (sysState.program == 0xFF) {
        return -1;
    }
    switch (arg0) {
    case 0:
        fn = sysProgramTable[sysState.program].entry;
        break;
    case 1:
        fn = sysProgramTable[sysState.program].unk14;
        break;
    case 2:
        fn = sysProgramTable[sysState.program].unk18;
        break;
    case 3:
        fn = sysProgramTable[sysState.program].unk1C;
        break;
    case 4:
        fn = sysProgramTable[sysState.program].draw;
        break;
    case 5:
        fn = sysProgramTable[sysState.program].update;
        break;
    case 6:
        fn = sysProgramTable[sysState.program].endFrame;
        break;
    case 7:
        fn = sysProgramTable[sysState.program].debugMenu;
        break;
    case 8:
        fn = sysProgramTable[sysState.program].buildDraw;
        break;
    }
    if (fn != NULL) {
        sysState.progCallback = arg0;
        ret = fn(arg1, 0);
    }
    return ret;
}
