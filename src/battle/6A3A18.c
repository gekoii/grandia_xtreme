#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxBindUpdate(BtlUnit *arg0) {
    FxBind *p = &fxBind[arg0->id];
    VuVec4 v;
    FxBindPart *e;
    FxBindPart *q;
    s32 i;
    s32 j;
    s32 k;
    s8 n;

    if (p->parts == NULL && (arg0->params->status & 0x2000)) {
        e = (FxBindPart *)MemAlloc16C(0x580);
        p->parts = e;
        if (e == NULL) {
            return;
        }
        k = 32;
        do {
            e->life = 0;
            k--;
            e++;
        } while (k > 0);
        p->next = 0;
    }
    if (p->parts == NULL) {
        return;
    }
    GphAnimSetSelect(ptclAnimSetTex38);
    if (arg0->params->status & 0x2000) {
        for (i = 1; i >= 0; i--) {
            q = p->parts;
            q += p->next;
            q->life = 16;
            BtlVecRandomDirInRange(&v, 0, 0xFFFF, 0, 0xFFFF);
            v.w = (rand() >> 15) * (1.0f / 65536.0f) * 8.0f + 2.0f;
            q->vel[0] = v.x * v.w;
            q->vel[1] = v.y * v.w;
            q->vel[2] = v.z * v.w;
            v.w = (rand() >> 15) * (1.0f / 65536.0f) * 50.0f + 70.0f;
            q->pos[0] = v.x * v.w;
            q->pos[1] = v.y * v.w;
            q->pos[2] = v.z * v.w;
            BtlChrGetPartPos(&v, arg0, 1);
            q->pos[0] += v.x;
            q->pos[1] += v.y;
            q->pos[2] += v.z;
            q->sx = ((rand() >> 15) * (1.0f / 65536.0f) * 25.0f + 15.0f) * 0.5f;
            q->sy = ((rand() >> 15) * (1.0f / 65536.0f) * 25.0f + 15.0f) * 0.5f;
            q->angle = rand() >> 15;
            q->flip = q->angle & 3;
            GphAnimStateInit(&q->anim);
            p->next++;
            if (p->next >= 32) {
                p->next = 0;
            }
        }
    }
    q = p->parts;
    n = 0;
    j = 32;
    do {
        if (q->life > 0) {
            q->pos[0] += q->vel[0];
            q->pos[1] += q->vel[1];
            q->pos[2] += q->vel[2];
            q->vel[0] *= 0.9f;
            q->vel[1] *= 0.9f;
            q->vel[2] *= 0.9f;
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
