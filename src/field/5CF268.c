#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"


void CamScrUpdate(void) {
    void *p;

    switch (camScrState) {
    case 0:
        EvtSystemFlagSet(0x82, 0);
        EvtSystemFlagSet(0x83, 0);
        CamFollowSyncToCamera(camField);
        return;
    case 2:
        EvtSystemFlagSet(0x82, 0);
        camScrState = CamScrExec(camScrState, camField, NULL);
        break;
    case 1:
        EvtSystemFlagSet(0x82, 0);
        EvtSystemFlagSet(0x83, 1);
        p = CamScrGet(camScrId);
        camScrState = CamScrExec(camScrState, camField, p);
        break;
    case 3:
        if (EvtSystemFlagGet(0x82) == 0) {
            camScrState = 2;
            camScrState = CamScrExec(camScrState, camField, NULL);
        }
        break;
    case 4:
        camScrState = 0;
        CamFollowSyncToCamera(camField);
        return;
    }
    switch (camScrState) {
    case 0:
        EvtSystemFlagSet(0x83, 0);
        CamFollowSyncToCamera(camField);
        return;
    case 2:
        EvtSystemFlagSet(0x82, 0);
        return;
    case 3:
        EvtSystemFlagSet(0x82, 1);
        return;
    case 4:
        camScrState = 0;
        CamFollowSyncToCamera(camField);
        return;
    default:
        return;
    }
}
