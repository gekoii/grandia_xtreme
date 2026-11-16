#include "common.h"
#include "save.h"
#include "field/n24.h"

void MiniConcertSaveResults(void) {
    MiniConcertWork *p = miniConcertWork;
    sysSaveData.unk1F44 = p->unk32.b[0];
    sysSaveData.unk1F45 = p->unk32.b[1];
    sysSaveData.unk1F46 = p->unk35;
    sysSaveData.unk1F47 = p->unk36;
    sysSaveData.unk1F5C = ((u8 *)&p->unk50.unk0)[0];
    sysSaveData.unk1F5D = ((u8 *)&p->unk50.unk0)[1];
    sysSaveData.unk1F6F = p->unk30.b[0];
    sysSaveData.unk1F70 = p->unk30.b[1];
}
