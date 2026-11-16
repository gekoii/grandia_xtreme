#include "common.h"
#include "protos/field.h"
#include "field/n24.h"

void MiniConcertComboMarksUpdate(void);

void MiniConcertPlayUpdate(void) {
    switch (miniConcertWork->subState) {
    case 1:
        MiniConcertPlayInit();
        MiniConcertCameraInit();
        miniConcertWork->frame = 0;
        miniConcertWork->subState = 2;
        EvtScnrCall(miniConcertData->rules->unkE);
        miniConcertWork->panels[0].alpha = 0;
        EvtGlobalFlagSet(0x40F, 0);
        break;
    case 2:
        if ((s8)miniConcertWork->panels[0].alpha >= 0) {
            miniConcertWork->panels[0].alpha += 4;
        }
        if (EvtScnrIsRunning() == 0) {
            if (EvtGlobalFlagGet(0x419) == 0) {
                EvtScnrCall(miniConcertData->rules->unk10);
                EvtGlobalFlagSet(0x419, 1);
            } else {
                FldSndBgmPlayFade(miniConcertWork->unk10);
                miniConcertWork->frame = 0;
                miniConcertWork->subState = 5;
            }
        }
        break;
    case 5:
        if ((s8)miniConcertWork->panels[0].alpha >= 0) {
            miniConcertWork->panels[0].alpha += 4;
        }
        if (FldSndBgmGetPlayPos() != 0) {
            miniConcertWork->frame = 0;
            miniConcertWork->subState = 3;
        }
        break;
    case 3:
        MiniConcertNotesSpawn();
        MiniConcertNotesUpdate();
        MiniConcertNoteHit();
        MiniConcertComboMarksUpdate();
        MiniConcertCarrosDance();
        MiniConcertSparksUpdate();
        MiniConcertBurstsUpdate();
        MiniConcertCameraUpdate();
        MiniConcertVibrationUpdate();
        if (miniConcertWork->frame == miniConcertWork->unk14) {
            FldSndBgmStop();
            EvtScnrCall(miniConcertData->rules->unk12);
        }
        if (miniConcertWork->frame > miniConcertWork->unk14) {
            if (miniConcertWork->panels[0].alpha != 0) {
                miniConcertWork->panels[0].alpha -= 2;
            }
            if (EvtScnrIsRunning() == 0) {
                miniConcertWork->frame = 0;
                miniConcertWork->subState = 4;
            }
        }
        break;
    case 4:
        miniConcertWork->frame = 0;
        miniConcertWork->subState = 0;
        miniConcertWork->state = 5;
        break;
    default:
        miniConcertWork->state = 5;
        break;
    }
    MiniSpritesUpdate(miniConcertWork->sprites, 0x119);
}
