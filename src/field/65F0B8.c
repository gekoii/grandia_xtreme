#include "common.h"
#include "pad.h"
#include "field/n26.h"
#include "protos/field.h"

s32 McLoadFlowUpdate(void) {
    s32 cur;

    mcLoadPad = padPort0;
    if (mcLoadPrevState != mcLoadState) {
        mcLoadStateChanged = 1;
    } else {
        mcLoadStateChanged = 0;
    }
    cur = mcLoadState;
    switch (cur) {
    case 0:
        McReset();
        McLoadSelectPort();
        break;
    case 2:
        McLoadSelectSlot();
        break;
    case 0x101:
        McLoadReadDir();
        break;
    case 0x104:
        McLoadScanSlots();
        break;
    case 3:
        McLoadRead();
        break;
    case 0x304:
        McLoadDoneMsg();
        break;
    case 0x30A:
        McLoadFailedMsg();
        break;
    case 0x100:
        McLoadCheckCard();
        break;
    case 0x308:
        McLoadNoDataMsg();
        break;
    case 0x309:
        McLoadCardOutMsg();
        break;
    case 0x800:
        mcLastSlot = mcLoadSlot;
        mcLastPort = mcLoadPort;
        FldPlayTimeSet(mcSaveDataPtr->playTime);
        break;
    case 0x801:
        break;
    default:
        DbgPrintf(0xF, 0xA, mcLoadBadStateFmt, mcLoadState);
        break;
    }
    mcLoadPrevState = cur;
    McLoadSync();
    if (mcLoadState != 0 && mcLoadState != 3 && mcLoadState != 0x30A && mcLoadState != 0x304 &&
        mcLoadState != 0x801 && mcLoadState != 0x800) {
        if (McGetCurPort()->state < 2) {
            McLoadSetErrorMsg(mcLoadSlotOutStr);
            McLoadSetState(0x309);
        }
    }
    if (mcLoadPrevState == 0x304 && mcLoadState == 0x309) {
        return 1;
    }
    return 0;
}
