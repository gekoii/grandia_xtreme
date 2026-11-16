#include "common.h"
#include "field/state.h"
#include "field/n15.h"

void CamBlendSetTarget(void) {
    camBlendTo = camField[0];
    if (camBlend.to.yaw < 0.0f) {
        camBlend.to.yaw += 6.283184f;
    } else if (camBlend.to.yaw >= 6.283184f) {
        camBlend.to.yaw -= 6.283184f;
    }
}
