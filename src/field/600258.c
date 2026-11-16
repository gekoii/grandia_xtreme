#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/n20.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void MapMiniInit(void) {
    MapMiniTex *s;

    mapMiniWork = FldMemAlloc(0x1E70);
    MemSet(mapMiniWork, 0, 0x1E70);
    mapMiniWork->texBufs[0] = FldMemAlloc(0x8000);
    mapMiniWork->texBufs[1] = FldMemAlloc(0x8000);
    mapMiniWork->texBufs[2] = FldMemAlloc(0x8000);
    mapMiniWork->clut = FldMemAlloc(0x40);
    MemSet(mapMiniWork->texBufs[0], 0, 0x8000);
    MemSet(mapMiniWork->texBufs[1], 0, 0x8000);
    MemSet(mapMiniWork->texBufs[2], 0, 0x8000);
    MemSet(mapMiniWork->clut, 0, 0x40);
    mapMiniWork->texSrc[0] = NULL;
    mapMiniWork->texSrc[1] = NULL;
    mapMiniWork->texSrc[2] = NULL;
    mapMiniPac = sysGrxsysPac->modelPack148;
    mapMiniWork->showDelay = 1;
    mapMiniWork->zoomFrom[0] = 1.5707964f;
    mapMiniWork->zoomFrom[1] = 800.0f;
    mapMiniWork->zoomFrom[2] = 10.0f;
    mapMiniWork->model = ChrModelCreate(FilePacGetModel(mapMiniPac, 0));
    ChrModelSetPartAttr(mapMiniWork->model, 2, 1);
    s = mapMiniPac->tex;
    s->pixels = mapMiniWork->texBufs[0];
    mapMiniWork->hdr = &mapMiniAutoHdr;
    s->clut = mapMiniWork->clut;
    mapMiniAutoHdr.floors[0].w = 0x40;
    mapMiniAutoHdr.floors[0].h = 0x40;
    mapMiniAutoHdr.floors[0].minY = 0.0f;
    mapMiniAutoHdr.floors[0].unk8 = 5.0f;
    mapMiniAutoHdr.floors[0].unkC = 5.0f;
}
