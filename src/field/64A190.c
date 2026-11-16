#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertSongSelectUpdate(void) {
    switch (miniConcertWork->subState) {
    case 1:
        MiniConcertSongSelectInit();
        miniConcertWork->frame = 0;
        miniConcertWork->subState = 2;
        break;
    case 2:
        miniConcertWork->frame = 0;
        miniConcertWork->subState = 3;
        break;
    case 3:
        if (MiniConcertSongSelectInput() == 1) {
            miniConcertWork->frame = 0;
            miniConcertWork->subState = 4;
        }
        break;
    case 4:
        miniConcertWork->frame = 0;
        miniConcertWork->state = 3;
        miniConcertWork->subState = 1;
        MiniPanelSlideOut(&miniConcertWork->menu.panels[0], 2);
        MiniPanelSlideOut(&miniConcertWork->menu.panels[1], 2);
        break;
    default:
        miniConcertWork->state = 5;
        miniConcertWork->subState = 0;
        break;
    }
    MiniSpritesUpdate(miniConcertWork->menu.sprites, 0xE);
}
