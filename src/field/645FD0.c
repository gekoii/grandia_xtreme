#include "common.h"
#include "field/state.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniUpdate(void) {
    if (miniCtl->dataFile != 0 && miniCtl->state != 0) {
        if (miniCtl->kind == 2) {
            switch (miniCtl->state) {
            case 1:
                if (EvtScnrIsRunning() == 0) {
                    miniCtl->state = 2;
                    ((s64 (*)(void))MiniBallStart)();
                }
                break;
            case 2:
                if (MiniBallUpdate() != 0) {
                    ((s64 (*)(void))MiniRequestEnd)();
                }
                break;
            case 3:
                MiniBallEnd();
                EvtScnrCall(miniCtl->endEvt);
                miniCtl->state = 0;
                break;
            }
        } else {
            switch (miniCtl->state) {
            case 1:
                if (EvtScnrIsRunning() == 0) {
                    EvtScnrCall(miniCtl->startEvt);
                    miniCtl->state = 2;
                    MiniConcertStart();
                    camCtl.aiType = 0xD;
                }
                break;
            case 2:
                if (MiniConcertUpdate() != 0) {
                    ((s64 (*)(void))MiniRequestEnd)();
                }
                break;
            case 3:
                MiniConcertEnd();
                EvtScnrCall(miniCtl->endEvt);
                miniCtl->state = 0;
                camCtl.aiType = camCtl.prevType;
                break;
            }
        }
    }
}
