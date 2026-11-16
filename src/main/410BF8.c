#include "common.h"
#include "protos/main.h"
#include "party.h"





void PtyUnitInitFromChar(PtyUnit *arg0, s16 arg1) {
    PtyCharDef *d;
    PtyChar *d2;
    s32 i;
    s8 *p;

    d = &ptyCharDefTable[arg1 - 0x1001];
    arg0->name = d->name;
    arg0->id = arg1;
    arg0->icon = arg1 - 1;
    arg0->size = 0;
    for (i = 1; i >= 0; i--) {
        p = &arg0->race[i];
        *p = 0;
    }
    arg0->type = 0;
    arg0->ipStopDmg = d->unk28;
    arg0->regMgcIpr = 1.0f;
    arg0->bitCsf = d->unitFlags | 0x400;
    arg0->ipStopDown = d->unk2A;
    arg0->regKb = (f32)(100 - d->unk3D) * 0.01f;
    arg0->colR = d->colR;
    arg0->colH = d->colH;
    arg0->runTime = d->unk1E;
    d2 = &ptyCharData[arg1 - 0x1001];
    arg0->restartEquip = d->unk22[0] * 0.0001f;
    arg0->restartGuard = d->unk22[1] * 0.0001f;
    arg0->restartEvade = d->unk22[2] * 0.0001f;
    arg0->exp = d2->exp;
    PtyCharUpdateLevel(arg0);
    arg0->hp = d2->hp;
    arg0->mp = 0;
    arg0->sp = d2->sp;
    arg0->status = d2->status;
}
