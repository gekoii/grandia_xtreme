#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/misc.h"
#include "pad.h"
#include "sys.h"


void BtlUnitStateGameOver(BtlUnit *arg0) {
    switch (arg0->subPhase) {
    case 0:
        btlBgmHandle.w = SndBgmPlay(0x1E, 1);
        BtlCamSetMode(0, 0x10, arg0, BtlCamGameOverCb);
        IpPanelSetMode(4, 0x12, 1);
        IpGaugeSetShown(0);
        btlPhase = 0x83;
        arg0->stateCount = 0;
        arg0->subPhase++;
    case 1:
        arg0->stateCount = 0;
        arg0->subPhase++;
    case 2:
        if (arg0->stateCount == 60) {
            SndStreamPlay(arg0->params->id * 2 - 0x1FE2, 0, 0);
        }
        if (arg0->stateCount >= 91 && ((padPort0Trigger & 0x60) || arg0->stateCount >= 1001)) {
            arg0->subPhase++;
        }
        break;
    case 3:
        SndBgmFadeVolume((u8)btlBgmHandle.w, 0xF0, 0.0f);
        arg0->stateCount = 0;
        arg0->subPhase++;
    case 4:
        if (arg0->stateCount >= 191) {
            arg0->subPhase++;
            btlHandoffResult = 3;
            btlExitRequest = 1;
        }
        break;
    }
}
