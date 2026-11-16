#include "common.h"
#include "battle/n30.h"

void IpPanelStatusIconSetActive(FxStatusIcon *arg0, s32 arg1) {
    if (arg1 != 0x2000) {
        if (arg1 != 0) {
            if (arg0->state <= 0) {
                arg0->state = 1;
                arg0->pos = 160.0f;
                arg0->alpha = 0;
                arg0->scale = 1.0f;
            }
        } else if (arg0->state > 0) {
            arg0->state = -1;
        }
    }
}
