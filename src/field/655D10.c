#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n25.h"

s32 WmapCheckEvent(void) {
    s32 r;
    s32 one;

    one = 1;
    if (wmapWork->eventDone == one) {
        return 0;
    }
    if (sysSaveData.mainFlag >= 11000 && sysSaveData.mainFlag < 11150 && wmapWork->cur == 2) {
        r = ChrIsInParty(2);
        if (r == one) {
            ChrPartySwapToSlot(2, 1);
            EvtScnrCall(0x2001);
            wmapWork->eventDone = r;
            wmapWork->nodes[2].mapNo = 0x4800;
            wmapWork->nodes[2].entrance = 0x1001;
            return 1;
        }
        EvtScnrCall(0x2000);
        return 2;
    }
    if (sysSaveData.mainFlag >= 11800 && sysSaveData.mainFlag < 11940 && wmapWork->cur == 3) {
        r = ChrIsInParty(4);
        if (r == 1) {
            ChrPartySwapToSlot(4, 1);
            EvtScnrCall(0x2003);
            wmapWork->eventDone = r;
            wmapWork->nodes[3].mapNo = 0x5000;
            wmapWork->nodes[3].entrance = 0x1001;
            return 1;
        }
        EvtScnrCall(0x2002);
        return 2;
    }
    if (sysSaveData.mainFlag >= 12170 && sysSaveData.mainFlag < 12190 && wmapWork->cur == 4) {
        r = ChrIsInParty(7);
        if (r == 1) {
            ChrPartySwapToSlot(7, 1);
            EvtScnrCall(0x2005);
            wmapWork->eventDone = r;
            wmapWork->nodes[4].mapNo = 0x5800;
            wmapWork->nodes[4].entrance = 0x1001;
            return 1;
        }
        EvtScnrCall(0x2004);
        return 2;
    }
    if (sysSaveData.mainFlag >= 12190 && sysSaveData.mainFlag < 12270 && wmapWork->cur == 4) {
        r = ChrIsInParty(7);
        if (r == 1) {
            ChrPartySwapToSlot(7, 1);
            EvtScnrCall(0x2005);
            wmapWork->eventDone = r;
            wmapWork->nodes[4].mapNo = 0x5800;
            wmapWork->nodes[4].entrance = 0x1000;
            return 1;
        }
        EvtScnrCall(0x2004);
        return 2;
    }
    return 0;
}
