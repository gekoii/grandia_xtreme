#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallSetupUpdate(void) {
    switch (miniBallWork->subState) {
    case 1:
        MiniBallCourseSelect();
        if (miniBallWork->game.course == 0) {
            miniBallWork->state = 5;
            return;
        }
        MiniBallPanelsInit();
        miniBallWork->frame = 0;
        miniBallWork->subState = 2;
        break;
    case 2:
        if (MiniBallDealBalls() == 1) {
            if (EvtGlobalFlagGet(0x41A) == 0) {
                EvtGlobalFlagSet(0x40F, 0);
                EvtScnrCall(miniBallData->params->setupScnr);
                EvtGlobalFlagSet(0x41A, 1);
            }
            miniBallWork->frame = 0;
            miniBallWork->subState = 3;
        }
        break;
    case 3:
        if (EvtScnrIsRunning() == 0) {
            EvtGlobalFlagSet(0x40F, 1);
            if (MiniBallChooseColor() == 1) {
                miniBallWork->subState = 4;
            }
        }
        break;
    case 4:
        MiniPanelSlideIn(&miniBallWork->game.panels[8], 10);
        miniBallWork->frame = 0;
        miniBallWork->state = 4;
        miniBallWork->subState = 1;
        break;
    default:
        miniBallWork->state = 5;
        break;
    }
    MiniBallBallsUpdate(miniBallWork->game.balls, 20);
}
