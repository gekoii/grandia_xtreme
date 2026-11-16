#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"
#include "pad.h"


void CamUpdate(Pad *pad) {
    if (camCtl.unk6 != 0) {
        camCtl.unk6 = 0;
        return;
    }
    if (CamScrIsRunning() != 0) {
        CamScrUpdate();
    } else {
        CamFollowUpdate(camField, camCtl.aiType);
    }
    if (camShakeCountdown == 0) {
        CamUpdateShake(camField);
        camShakeCountdown = camShakeInterval;
    } else {
        camShakeCountdown--;
    }
}
