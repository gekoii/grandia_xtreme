#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"


s32 BtlUnitInitFromDef(BtlUnit *u, s32 id) {
    BtlCharDef *e;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    f32 z;

    e = btlCharDefs;
    for (i = 0; i < 8; i++, e++) {
        if (e->id == id) {
            break;
        }
    }
    if (i >= 8) {
        return 0;
    }
    /* Access casts (s32 / void * stores, s32 setState load): their alias sets give the original store order */
    *(s32 *)&u->hookPre = 0;
    *(s32 *)&u->hookPost = 0;
    *(s32 *)&u->hookDraw = 0;
    ((s32 *)u->pad180)[0] = 0;
    ((s32 *)u->pad180)[1] = 0;
    u->unk188 = 0;
    u->unk18C = 0;
    *(void **)&u->state = (void *)0x80;
    *(void **)&u->subPhase = NULL;
    *(void **)&u->phase = (void *)1;
    u->def = e;
    ((void (*)(BtlUnit *))*(s32 *)&e->setState)(u);
    u->pos[3] = 0;
    z = u->pos[3];
    u->unkD0[0] = u->unkD0[1] = u->unkD0[2] = z;
    u->frameSpeed = 1.0f;
    u->rot[0] = u->rot[1] = u->rot[2] = 0;
    u->turnLeft = 0;
    u->scale[0] = u->scale[1] = u->scale[2] = 1.0f;
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(u->ambient) : "memory");
    u->alpha = 1.0f;
    u->alphaStep = z;
    u->dust = z;
    u->rotAccel = 0x40;
    u->rotVel = 0x80;
    u->rotVelMax = 0x400;
    u->ipDamage = z;
    u->ipRate = 1.0f;
    u->attacker = u->id;
    u->action = 0x7F;
    u->flag = 2;
    u->comboLeader = -1;
    u->ipMode = 0;
    u->ipStop = 0;
    u->stateCount = 0;
    u->flags = 0;
    u->run = 0;
    u->scopeType = 0;
    u->mgcProg = NULL;
    u->ai = 0;
    u->turn = 0;
    for (j = 0; j < 3; j++) {
        u->comboPartners[j] = -1;
    }
    u->exeSp = rand() >> 24;
    u->exeGood = rand() >> 24;
    u->exeBad = rand() >> 24;
    u->transp = 0;
    u->antiMagic = 0;
    u->regene = 0;
    u->sleep = 0;
    u->confu = 0;
    u->sealS = 0;
    u->sealM = 0;
    u->bind = 0;
    u->vanish = 0;
    for (k = 0; k < 5; k++) {
        u->statMod[k] = 0;
    }
    u->powUp = 0;
    u->wall = 0;
    u->mvp = 0;
    u->item = 0;
    u->treasure = 0;
    u->itemRate = 0;
    for (m = 0; m < 3; m++) {
        u->partHooks[m] = NULL;
        u->parts[m] = -1;
    }
    u->body = -1;
    u->partsChr = -1;
    u->rtAct = 0;
    u->rtP = 0;
    u->params->angryProb = 0;
    return 1;
}
