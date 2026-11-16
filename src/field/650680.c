#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallEntryUpdate(void) {
    MiniBallWork *p;

    switch (miniBallWork->subState) {
    case 1:
        miniBallWork->game.gold = PtyGetGold();
        MiniPanelInit(&miniBallWork->game.panels[8], &miniBallData->panels[8], miniBallData->cells, miniBallWork->game.sprites);
        MiniSpriteSetMsgGroup(&miniBallWork->game.sprites[53], 11, 6);
        MiniSpriteSetTextSize(&miniBallWork->game.sprites[53], 20, 20);
        MiniSpriteSetNumber(&miniBallWork->game.sprites[53], miniBallWork->game.gold, 0);
        MiniSpriteSetNumberSize(&miniBallWork->game.sprites[53], 20, 20);
        miniBallWork->game.sprites[53].numOfsX = 0x48;
        miniBallWork->game.sprites[53].numOfsY = 1;
        miniBallWork->game.panels[8].showNumbers = 1;
        MiniBallSetRank();
        miniBallWork->frame = 0;
        miniBallWork->subState = 2;
        break;
    case 2:
        EvtGlobalFlagSet(0x40F, 0);
        EvtScnrCall(miniBallData->params->entryScnr);
        p = miniBallWork;
        p->subState = 3;
        p->frame = 0;
        break;
    case 3:
        miniBallWork->game.gold = PtyGetGold();
        if (EvtScnrIsRunning() == 0) {
            EvtGlobalFlagSet(0x40F, 1);
            miniBallWork->subState = 4;
        }
        break;
    case 4:
        p = miniBallWork;
        p->frame = 0;
        p->state = 3;
        miniBallWork->subState = 1;
        MiniPanelSlideOut(&miniBallWork->game.panels[8], 2);
        break;
    default:
        miniBallWork->state = 5;
        break;
    }
}
