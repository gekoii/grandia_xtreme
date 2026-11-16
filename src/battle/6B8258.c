#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "gph.h"
#include "main/n05.h"
#include "battle/env.h"


void EnvStageLoad(void) {
    ChrModelPool st;
    ChrModelData *res;
    s32 h;
    GphPart *n;
    EnvStageZone *z;

    res = FilePacGetModel(btlHandoff.stagePack, btlHandoff.unkB4);
    BtlModelPoolReserve(res, 1);
    BtlModelPoolAlloc(&st);
    envStageModel = BtlModelCreate(res, 1);
    ChrModelPoolSet(NULL);
    h = res->gaObjCount;
    envStageDrawList = MemAlloc32(GphDrawListSize(h));
    GphDrawListInit(h, envStageDrawList);
    GphDrawListSet(envStageDrawList);
    GphDrawListClear(0);
    n = BtlChrFindPart(envStageModel, 1);
    envStageZoneCount = 0;
    if (n != NULL && n->child != 0) {
        n += n->child;
        for (;;) {
            envStageZoneCount++;
            if (n->next == 0) break;
            n += n->next;
        }
        envStageZones = MemAlloc16A(envStageZoneCount << 4);
        n = BtlChrFindPart(envStageModel, 1);
        z = envStageZones;
        n += n->child;
        for (;;) {
            EnvStageZoneInit(n, z);
            z->part = n;
            if (n->next == 0) {
                break;
            }
            z++;
            n += n->next;
        }
    }
    SysSetClearColor(0x80000000);
    envStageShadeQuad.w = 0x280;
    envStageShadeAlpha = 0.0f;
    envStageShadeStep = 0.0f;
    envStageShadeQuad.h = 0x1C0;
    envStageShadeQuad.originX = 0;
    envStageShadeQuad.originY = 0;
    envStageShadeQuad.col[3].w = 0x80;
    envStageShadeQuad.col[1].w = 0x80;
    envStageShadeQuad.col[2].w = 0x80;
    envStageShadeQuad.col[0].w = 0x80;
}
