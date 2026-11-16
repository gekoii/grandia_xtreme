#include "common.h"
#include "protos/field.h"
#include "field/n24.h"

void MiniConcertStageSetupInit(void);

void MiniConcertStageSetupUpdate(void) {
    switch (miniConcertWork->subState) {
    case 1:
        MiniConcertStageSetupInit();
        miniConcertWork->frame = 0;
        miniConcertWork->subState = 2;
        break;
    case 2:
        miniConcertWork->frame = 0;
        miniConcertWork->subState = 3;
        break;
    case 3:
        if (MiniConcertStageMenuInput() == 1) {
            miniConcertWork->frame = 0;
            miniConcertWork->subState = 4;
        }
        break;
    case 4:
        miniConcertWork->frame = 0;
        miniConcertWork->state = 4;
        miniConcertWork->subState = 1;
        MiniPanelSlideOut(&miniConcertWork->stage.panels[0], 2);
        MiniPanelSlideOut(&miniConcertWork->stage.panels[1], 2);
        MiniPanelSlideOut(&miniConcertWork->stage.panels[2], 2);
        MiniPanelSlideOut(&miniConcertWork->stage.panels[3], 2);
        break;
    default:
        miniConcertWork->state = 5;
        miniConcertWork->subState = 0;
        break;
    }
    MiniSpritesUpdate(miniConcertWork->stage.sprites, 0x40);
}
