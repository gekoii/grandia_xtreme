#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxStunUpdate(BtlUnit *arg0) {
    FxStun *p = &fxStun[arg0->id];
    FxStunPart *e;
    FxStunPart *q;
    s32 i;
    s32 j;
    s8 n;

    if (p->parts == NULL && (arg0->params->status & 0x80)) {
        e = (FxStunPart *)MemAlloc16C(0x100);
        p->parts = e;
        if (e == NULL) {
            return;
        }
        i = 16;
        do {
            e->life = 0;
            i--;
            e++;
        } while (i > 0);
        p->next = 0;
        p->angle = rand() >> 15;
    }
    if (p->parts == NULL) {
        return;
    }
    GphAnimSetSelect(ptclAnimSetTex38);
    p->angle -= 0x16C;
    if (arg0->params->status & 0x80) {
        q = p->parts;
        q += p->next;
        q->life = 16;
        q->angle = rand() >> 15;
        q->scale = ((rand() >> 15) * (1.0f / 65536.0f) * 15.0f + 10.0f) * 0.5f;
        GphAnimStateInit(&q->anim);
        p->next++;
        if (p->next >= 16) {
            p->next = 0;
        }
    }
    q = p->parts;
    n = 0;
    j = 16;
    do {
        if (q->life > 0) {
            GphAnimStep(&q->anim, 1.0f);
            q->life--;
            n++;
        }
        j--;
        q++;
    } while (j > 0);
    if (n == 0) {
        MemFree(p->parts);
        p->parts = NULL;
    }
}
