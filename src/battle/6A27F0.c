#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxRegenUpdate(BtlUnit *arg0) {
    FxRegen *p = &fxRegen[arg0->id];
    VuVec4 dir;
    VuVec4 pos;
    FxRegenPart *e;
    s32 i;
    FxRegenPart *q;
    s32 j;

    if (p->parts == NULL && (arg0->params->status & 8)) {
        p->delay--;
        if (p->delay <= 0) {
            q = (FxRegenPart *)MemAlloc16C(0x3E0);
            p->parts = q;
            if (q == NULL) {
                p->delay = 0;
                return;
            }
            e = q;
            BtlChrGetPartPos(&pos, arg0, 1);
            i = 31;
            do {
                i--;
                e->angle = rand() >> 15;
                e->scale = ((rand() >> 15) * (1.0f / 65536.0f) * 10.0f + 13.0f) * 0.5f;
                BtlVecRandomDirInRange(&dir, 0, 0x8000, 0, 0xFFFF);
                e->vel[0] = dir.x * 10.0f;
                e->vel[1] = dir.y * 10.0f;
                e->vel[2] = dir.z * 10.0f;
                e->pos[0] = pos.x + dir.x * 50.0f;
                e->pos[1] = pos.y + dir.y * 50.0f;
                e->pos[2] = pos.z + dir.z * 50.0f;
                e++;
            } while (i > 0);
            p->timer = 30;
            p->delay = 37;
        }
    }
    if (p->parts == NULL) {
        return;
    }
    q = p->parts;
    j = 31;
    do {
        j--;
        q->pos[0] += q->vel[0];
        q->pos[1] += q->vel[1];
        q->pos[2] += q->vel[2];
        q->vel[0] *= 0.95f;
        q->vel[1] *= 0.95f;
        q->vel[2] *= 0.95f;
        q++;
    } while (j > 0);
    p->timer--;
    if (p->timer <= 0) {
        MemFree(p->parts);
        p->parts = NULL;
    }
}
