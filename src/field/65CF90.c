#include "common.h"
#include "pad.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveFlowUpdate(void) {
    s32 state;
    s32 cur;

    mcSavePad = padPort0;
    mcSavePlayTimeNow = FldPlayTimeGet();
    if (mcSavePrevState != mcSaveState) {
        mcSaveStateChanged = 1;
    } else {
        mcSaveStateChanged = 0;
    }
    state = mcSaveState;
    switch (state) {
    case 0:
        McReset();
        McSaveSelectPort();
        break;
    case 2:
        McSaveSelectSlot();
        break;
    case 0x101:
        McSaveReadDir();
        break;
    case 0x102:
        McSaveCheckGameFiles();
        break;
    case 0x104:
        McSaveScanSlots();
        break;
    case 0x200:
        McSaveCheckSpaceNew();
        break;
    case 0x201:
        McSaveCheckSpace();
        break;
    case 0x202:
        McSaveWaitIdle();
        break;
    case 0x300:
        McSaveNoSpaceMsg();
        break;
    case 0x301:
        McSaveBrokenDataMsg();
        break;
    case 0x302:
        McSaveNoSpaceMsg2();
        break;
    case 0x303:
        McSaveWriteFailedMsg();
        break;
    case 0x304:
        McSaveDoneMsg();
        mcSaveDone = 1;
        break;
    case 3:
        McSaveWrite();
        break;
    case 4:
        McSaveFormat();
        break;
    case 0x305:
        McSaveAskFormat();
        break;
    case 0x100:
        McSaveCheckCard();
        break;
    case 0x307:
        McSaveFormatFailedMsg();
        break;
    case 0x308:
        McSaveConfirmOverwrite();
        break;
    case 0x309:
        McSaveCardOutMsg();
        break;
    case 0x30A:
        McSaveNotPs2CardMsg();
        break;
    case 0x30B:
        McSaveConfirmQuit();
        break;
    case 0x800:
        break;
    default:
        DbgPrintf(0xF, 0xA, mcSaveBadStateFmt, mcSaveState);
        break;
    }
    mcSavePrevState = state;
    McSaveSync();
    cur = mcSaveState;
    if (cur == 0 || cur == 0x309 || cur == 3 || cur == 0x303 || cur == 4 || cur == 0x307 || cur == 0x30B) {
        return;
    }
    if (cur != 0x800 && McGetCurPort()->state < 2) {
        McSaveSetState(0x309);
    }
}
