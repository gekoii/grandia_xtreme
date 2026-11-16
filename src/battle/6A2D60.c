#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxAntiMagicUpdate(BtlUnit *arg0) {
    FxAntiMagic *p = &fxAntiMagic[arg0->id];
    FxAntiMagicPart *e;
    FxAntiMagicPart *q;
    FxAntiMagicPart *r;
    s32 i;
    s32 j;
    s8 n;

    if (p->parts == NULL && (arg0->params->status & 2)) {
        e = (FxAntiMagicPart *)MemAlloc16C(0xF0);
        p->parts = e;
        if (e == NULL) {
            return;
        }
        i = 30;
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
    p->angle += 0x222;
    if (arg0->params->status & 2) {
        r = &p->parts[p->next];
        r->life = 30;
        r->angle = rand() >> 15;
        r->scale = ((rand() >> 15) * (1.0f / 65536.0f) * 5.0f + 15.0f) * 0.5f;
        p->next++;
        if (p->next >= 30) {
            p->next = 0;
        }
    }
    q = p->parts;
    n = 0;
    j = 30;
    do {
        if (q->life > 0) {
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
