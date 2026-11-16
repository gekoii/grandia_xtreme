#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "battle/unit.h"

/* View of the bound ChrPack: +0x10 enemyData, +0x84 motions[20..31], +0x104
 * motionEvents[20..31] (s32 in ChrPack). */
typedef struct Data {
    u8 pad0[0x10];
    BtlChrData *unk10;
    u8 pad14[0x70];
    void *q[12];
    u8 padB4[0x50];
    void *s[12];
} Data;


void BtlCharDefSetup(BtlCharDef *e, s32 n) {
    ChrPackBinding *src;
    Data *d;
    s32 i;

    if (e->id > 0x1000) {
        src = btlHandoff.partyModels[n];
    } else {
        src = btlHandoff.enemyModels[n];
        for (i = 0; i < 2; i++) {
            e->dieParts2[i] = 0;
            e->dieParts[i] = 0;
        }
    }
    e->modelPack = (ChrModelData *)FilePacGetModel(src->modelPac, 0);
    d = (Data *)src->pack;
    e->data = d->unk10;
    e->motions[5] = d->q[0];
    e->motionEvents[5] = d->s[0];
    e->motions[7] = d->q[1];
    e->motionEvents[7] = d->s[1];
    e->motions[8] = d->q[2];
    e->motionEvents[8] = d->s[2];
    e->motions[9] = d->q[3];
    e->motionEvents[9] = d->s[3];
    e->motions[10] = d->q[4];
    e->motionEvents[10] = d->s[4];
    e->motions[6] = d->q[5];
    e->motionEvents[6] = d->s[5];
    e->motions[3] = d->q[6];
    e->motionEvents[3] = d->s[6];
    e->faceData = src;
    e->motions[0] = d->q[7];
    e->motionEvents[0] = d->s[7];
    e->motions[4] = d->q[8];
    e->motionEvents[4] = d->s[8];
    e->motions[11] = d->q[9];
    e->motionEvents[11] = d->s[9];
    e->motions[1] = d->q[10];
    e->motionEvents[1] = d->s[10];
    e->motions[2] = d->q[11];
    e->motionEvents[2] = d->s[11];
}
