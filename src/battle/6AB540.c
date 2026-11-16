#include "common.h"
#include "protos/battle.h"
#include "main/n05.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "battle/unit.h"


void PtclBurstSpawn(s32 type, BtlUnit *u, f32 *pos)
{
    ChrModelPool buf;
    PtclBurst *e;
    PtclDebris *pp;
    const PtclBurstType *k;
    s32 n;
    s32 i;
    s32 j;

    if (type == 0) {
        type = 9;
    }
    e = ptclBursts;
    for (i = 0; i < 16; i++, e++) {
        if (e->type == 0) {
            break;
        }
    }
    if (i >= 16) {
        return;
    }
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(e->ofs), "r"(pos), "r"(u->pos) : "memory");
    e->ofs[3] = 1.0f;
    if (type == 10) {
        n = ChrModelCalcInstanceSize(ptclDebrisModel) * 8;
        e->unk2C = BtlAlloc128(n);
        if (e->unk2C == NULL) {
            PtclBurstFree(e);
            return;
        }
        ChrModelPoolInit(&buf, e->unk2C, n);
        e->parts = BtlAlloc32(0x100);
        if (e->parts == NULL) {
            PtclBurstFree(e);
            return;
        }
        e->cb30 = (PtclBurstCb)PtclBurstUpdateDebris;
        e->cb34 = (PtclBurstCb)PtclBurstRegisterDebris;
        ChrModelPoolSet(&buf);
        pp = e->parts;
        for (j = 7; j >= 0; j--) {
            pp->model = BtlModelCreate(ptclDebrisModel, 0);
            pp->model->pos[0] = pos[0];
            pp->model->pos[1] = pos[1];
            pp->model->pos[2] = pos[2];
            pp->vel[0] = ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 8.0f;
            pp->vel[1] = (rand() >> 15) * (1.0f / 65536.0f) * 16.0f;
            pp->vel[2] = ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 8.0f;
            pp->model->rot[0] = rand() >> 15;
            pp->model->rot[1] = rand() >> 15;
            pp->model->rot[2] = rand() >> 15;
            pp->rotVel[0] = ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 16384.0f;
            pp->rotVel[1] = ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 16384.0f;
            pp->rotVel[2] = ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 16384.0f;
            pp->model->scale[0] = pp->model->scale[1] = pp->model->scale[2] = (rand() >> 15) * (1.0f / 65536.0f) + 1.0f;
            pp->life = (rand() >> 15) * (1.0f / 65536.0f) * 0.4f + 1.0f;
            pp++;
        }
        ChrModelPoolSet(NULL);
    } else if (type >= 13 && type <= 24) {
        if (type == 13 || type == 17 || type == 21) {
            e->rate = 4.5f;
            e->rate2 = 2.0f;
        }
        if (type == 14 || type == 18 || type == 22) {
            e->rate = 2.5f;
            e->rate2 = 1.0f;
        } else {
            e->rate = 1.5f;
            e->rate2 = 1.0f;
        }
        e->texId = 0x36;
        e->max = e->rate * 6.0f;
        e->max2 = e->rate2 * 6.0f;
        e->parts = BtlAlloc32(e->max << 6);
        if (e->parts == NULL) {
            PtclBurstFree(e);
            return;
        }
        if (type != 16 && type != 20 && type != 24) {
            e->parts2 = BtlAlloc32(e->max2 << 6);
            if (e->parts2 == NULL) {
                PtclBurstFree(e);
                return;
            }
        }
        e->cb30 = (PtclBurstCb)PtclBurstUpdateSpark;
        e->cb38 = (PtclBurstCb)PtclBurstDrawSpark;
        PtclBtlSprReset(e->parts, e->max);
        if (e->parts2 != NULL) {
            PtclBtlSprReset(e->parts2, e->max2);
        }
    } else {
        k = &ptclBurstTypeTable[type];
        e->rate = ptclBurstTypeTable[type].unk4;
        e->texId = k->tex;
        e->max = (s8)k->unk3;
        e->parts = BtlAlloc32(e->max << 6);
        if (e->parts == NULL) {
            PtclBurstFree(e);
            return;
        }
        e->cb30 = (PtclBurstCb)PtclBurstUpdateBasic;
        e->cb38 = (PtclBurstCb)PtclBurstDrawBasic;
        PtclBtlSprReset(e->parts, e->max);
    }
    e->unk3C = u;
    e->type = type;
    e->active = -1;
    e->timer = e->timer2 = 0.0f;
    e->count2 = 0;
    e->count = 0;
    e->unk4 = 0;
}
