#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallPlayUpdate(void) {
    MiniSprite *o;
    MiniBallWork *p;
    u8 k;

    switch (miniBallWork->subState) {
    case 1:
        MiniBallPlayBegin();
        break;
    case 2:
        FldSndBgmPlayFade(miniBallData->set->bgm);
        p = miniBallWork;
        p->subState = 5;
        p->frame = 0;
        break;
    case 5:
        if (FldSndBgmGetPlayPos() != 0) {
            MiniMsgBoxSet(miniBallWork->game.msgBox, 3);
            o = &miniBallWork->game.sprites[58];
            MiniSpriteSetNumber(o, miniBallWork->game.cost, 1);
            MiniSpriteSetNumberSize(o, 20, 20);
            o->numOfsX = 150;
            o->numOfsY = 2;
            o->num->gapOn = 1;
            o->num->gapAt = 12;
            o->num->gapWidth = (s32)((f32)(o->num->len * 20) * 0.5f + 2.0f);
            miniBallWork->game.panels[9].showNumbers = 1;
            miniBallWork->frame = 0;
            miniBallWork->subState = 3;
        }
        break;
    case 3:
        MiniBallShootInput();
        MiniBallPickWinner();
        MiniBallJudge();
        MiniBallReelsUpdate();
        MiniBallCheckWin();
        break;
    case 4:
        if (EvtScnrIsRunning() == 0) {
            k = miniBallUi->input.circleWait;
            if (k == 5) {
                MiniBallGivePrize();
                p = miniBallWork;
                p->subState = 0;
                p->frame = 0;
                miniBallWork->state = k;
            }
        }
        break;
    default:
        miniBallWork->state = 5;
        break;
    }
    MiniBallBallsUpdate(miniBallWork->game.balls, 20);
}
