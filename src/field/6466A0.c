#include "common.h"
#include "save.h"
#include "field/state.h"
#include "field/n24.h"

void MemSet(void *, s32, s32);
void MiniConcertCarrosGet(ChrObj *out[5][5]);

void MiniConcertStart(void) {
    MemSet(miniConcertWork, 0, 0x8CC0);
    MemSet(miniConcertFx, 0, 0x9810);
    miniConcertWork->state = 1;
    miniConcertWork->subState = 0;
    MiniConcertCarrosGet(miniConcertWork->carros);
    miniConcertWork->unk32.b[0] = sysSaveData.unk1F44;
    miniConcertWork->unk32.b[1] = sysSaveData.unk1F45;
    miniConcertWork->unk30.b[0] = sysSaveData.unk1F6F;
    miniConcertWork->unk30.b[1] = sysSaveData.unk1F70;
    miniConcertWork->unk30.h++;
    if (miniConcertWork->unk30.h >= 10000) {
        miniConcertWork->unk30.h = 9999;
    }
}
