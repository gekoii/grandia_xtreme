#include "common.h"
#include "pad.h"
#include "field/n26.h"
#include "protos/field.h"

s32 McSpaceCheckPort(void) {
    s32 state;

    mcSpaceCheckPad = padPort0;
    if (mcSpaceCheckStateChanged != 0) {
        mcSpaceCheckPortState = 0;
        mcSpaceCheckPortPrevState = 0xFFFF;
    }
    if (mcSpaceCheckPortPrevState != mcSpaceCheckPortState) {
        mcSpaceCheckPortStateChanged = 1;
    } else {
        mcSpaceCheckPortStateChanged = 0;
    }
    state = mcSpaceCheckPortState;
    switch (state) {
    case 0:
        McSpaceCheckCardType();
        break;
    case 0x401:
        break;
    case 0x402:
        McSpaceCheckReadDir();
        break;
    case 0x408:
        McSpaceCheckFreeKB();
        break;
    case 0x409:
        McSpaceCheckFreeKBWithSaves();
        break;
    case 0x405:
        mcSpaceCheckPortState = 0;
        break;
    case 0x406:
        mcSpaceCheckPortState = 0;
        break;
    case 0x40A:
        mcSpaceCheckPortState = 0;
        break;
    case 0x40B:
        mcSpaceCheckPortState = 0;
        break;
    }
    mcSpaceCheckPortPrevState = state;
    switch (state) {
    case 0x406:
        return -2;
    case 0x40A:
        return 1;
    case 0x405:
    case 0x40B:
        return -1;
    }
    return 0;
}
