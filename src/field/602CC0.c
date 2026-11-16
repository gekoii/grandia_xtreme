#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void MemSet(void *, s32, s32);
void AmapTemplateListsBuild(AmapWork *m);
void AmapTilesFixWallGimmicks(AmapWork *m);
void AmapBuildCollision(AmapWork *m, s32 a);
void AmapBuildMonsters(AmapWork *m);

extern u8 amapTypeFlagTable[];

void AmapGenerate(AmapWork *m) {
    s32 i;
    u8 *buf;
    s64 id; /* s64: as s32, u16 or u32 the whole function allocates differently */
    s32 r;
    f32 *v;

    /* carve the work buffer into its tables: byte sizes, as most of the tables have no element type */
    buf = m->tiles;
    m->unk124 = buf + m->width * m->height;
    m->objs = m->unk124 + m->width * m->height * 7;
    m->areas = (u8 *)m->objs + 0x1000;
    m->occ = m->areas + 0x4800;
    m->pathRanges = (void *)((u8 *)m->occ + 0x10000);
    m->pathPoints = (void *)((u8 *)m->pathRanges + 0x2000);
    m->unk158 = (u8 *)m->pathPoints + 0x30000;
    m->unk15C = m->unk158 + 0x2000;
    m->unk160 = m->unk15C + 0x30000;
    m->unk164 = m->unk160 + 0x2000;
    m->unk144 = m->unk164 + 0x30000;
    m->unk148 = m->unk144 + 0x3000;
    m->unk14C = m->unk148 + 0x1A00;
    m->treasures = m->unk14C + 0x3000;
    m->gimmicks = (void *)((u8 *)m->treasures + 0x2000);
    m->items = (AmapItemPlace *)((u8 *)m->gimmicks + 0xC00);
    m->monsters = (AmapPlace *)((u8 *)m->items + 0x180);
    m->sndEmitters = (void *)((u8 *)m->monsters + 0x180);
    m->entrances = (void *)((u8 *)m->sndEmitters + 0x400);
    m->spots = (void *)((u8 *)m->entrances + 0x400);
    m->ptList = (void *)((u8 *)m->spots + 0x610);
    m->unk17C = (u8 *)m->ptList + 0x2008;
    m->occs = m->unk17C + 0x618;
    m->lights = (void *)((u8 *)m->occs + 0x2500);
    amapNodes = (void *)((u8 *)m->lights + 0x400);
    m->unk188 = (u8 *)amapNodes + 0x600;

    for (i = 0; i < m->bufSize; i++) {
        buf[i] = 0;
    }
    for (i = 0; i < 0x40; i++) {
        EvtFlagSetByType(5, i, 0);
    }
    m->tmplFlags = 0;
    m->objCount = 0;
    m->areaCount = 0;
    m->gimmickCount = 0;
    m->itemCount = 0;
    m->monsterCount = 0;
    m->nodeCount = 0;
    m->unk1D = 0;
    m->groupCount = 0;
    m->alcoveCount = 0;
    m->startFixed = 0;
    m->exitFixed = 0;
    m->pathCount = 0;
    m->unkF6 = 0;
    m->unkF8 = 0;
    m->unkFA = 0;
    m->sndEmitterCount = 0;
    m->entranceCount = 0;
    m->occsCount = 0;
    m->lightCount = 0;
    m->mapType = 0;
    m->voidFilled = 0;
    MemSet(m->unk18C, 0, 0x1F8);
    amapForceTemplates[0] = 0;
    m->specialRooms = 0;
    m->tmplLogCount = 0;
    AmapCellMapInit(m);
    m->ptList->count = 0;
    amapNodeCount = 0;
    AmapSetupFloorParams(m);
    AmapCellMapInit(m);
    switch (m->nodeDensity) {
    case 0:
        m->nodeCount = m->width * m->height / 40;
        break;
    case 1:
        m->nodeCount = m->width * m->height / 30;
        break;
    default:
        m->nodeCount = m->width * m->height / 30;
        break;
    case 2:
        m->nodeCount = m->width * m->height / 20;
        break;
    }
    if (m->algo1 == 0) {
        m->algo1 = VuRandInt(7) + 1;
    }
    id = m->mapNo;
    m->mapType = 0;
    if ((id & 0xFF00) == 0xA000) {
        m->mapType = 1;
        if ((u16)id == 0xA008) {
            m->mapType = 2;
            m->algo2 = 8;
        }
    }
    if ((id & 0xFFF0) == 0xD800) m->mapType = 3;
    if ((id & 0xFFF0) == 0xD810) m->mapType = 4;
    if ((id & 0xFFF0) == 0xD820) m->mapType = 5;
    if ((id & 0xFFF0) == 0xD830) m->mapType = 6;
    if ((id & 0xFFF0) == 0xD840) m->mapType = 7;
    AmapTemplateListsBuild(m);
    r = VuRandInt(100) + 1;
    switch (m->algo1) {
    case 1:
        m->algo2 = 1;
        if (r < 50) m->algo2 = 2;
        break;
    case 2:
        m->algo2 = 3;
        if (r < 50) m->algo2 = 4;
        break;
    case 3:
        m->algo2 = 5;
        if (r < 50) m->algo2 = 6;
        break;
    case 4:
        if (r < 20) m->algo2 = 7;
        else if (r >= 41) m->algo2 = 8;
        else if (r < 60) m->algo2 = 9;
        else if (r < 80) m->algo2 = 10;
        else m->algo2 = 11;
        break;
    case 5:
        m->algo2 = 12;
        break;
    case 6:
        m->algo2 = 13;
        break;
    case 7:
        m->algo2 = 14;
        break;
    case 8:
        if (m->floor & 1) m->algo2 = 16;
        else m->algo2 = 15;
        break;
    default:
        m->algo2 = 0;
        break;
    }
    if (amapForceAlgo1 != -1) {
        m->algo1 = amapForceAlgo1;
        m->algo2 = amapForceAlgo2;
    }
    /* the template layouts are called with an s32 parameter (cast): their s16 one would sign-extend AmapTemplatePick's result */
    switch (m->algo2) {
    case 0:
    case 1:
    default:
        AmapGenLayoutOpen(m);
        break;
    case 2:
        AmapGenLayoutOpenDense(m);
        break;
    case 3:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom1Door)(m, AmapTemplatePick(m, 0, -1));
        break;
    case 4:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom2Door)(m, AmapTemplatePick(m, 1, -1));
        break;
    case 5:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom1Door)(m, AmapTemplatePick(m, 2, -1));
        break;
    case 6:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom2Door)(m, AmapTemplatePick(m, 3, -1));
        break;
    case 7:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom1Door)(m, AmapTemplatePick(m, 4, -1));
        break;
    case 8:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom2Door)(m, AmapTemplatePick(m, 5, -1));
        break;
    case 9:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom3Door)(m, AmapTemplatePick(m, 6, -1));
        break;
    case 10:
        ((void (*)(AmapWork *, s32))AmapGenLayoutRoom4Door)(m, AmapTemplatePick(m, 7, -1));
        break;
    case 11:
        ((void (*)(AmapWork *, s32))AmapGenLayoutSplit)(m, AmapTemplatePick(m, 8, -1));
        break;
    case 12:
        AmapGenLayoutQuadRooms(m);
        break;
    case 13:
        AmapGenLayoutQuadRoomsMedium(m);
        break;
    case 14:
        AmapGenLayoutGridRooms(m);
        break;
    case 15:
        m->unk1D = 1;
        m->nodeCount = m->width * m->height / 40;
        AmapGenLayoutFixedA(m);
        break;
    case 16:
        m->unk1D = 1;
        m->nodeCount = m->width * m->height / 40;
        AmapGenLayoutFixedB(m);
        break;
    }
    AmapAlcovesPlace(m);
    AmapVoidFillEnclosed(m);
    AmapVoidGrow(m);
    AmapFloorFixGaps(m);
    if (m->unk116) AmapVoidCarveWallSide(m);
    if (m->mapType == 2) AmapTilesBuild(m, 0);
    else AmapTilesBuild(m, 1);
    AmapStartExitReserve(m);
    AmapTilesRandomize(m);
    AmapNodesMaskSet();
    AmapStartFacingSet(m);
    AmapDeadEndMonstersPlace(m);
    AmapDeadEndItemsPlace(m);
    AmapMonstersScatter(m);
    AmapPlaceSpots(m);
    switch (m->mapType) {
    default:
    case 0:
        AmapGimmicksScatter(m, 2, 1, 1, 0x10, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xF, 1, 8);
        AmapGimmicksScatter(m, 3, 0, 1, 0x12, 1, 8);
        AmapGimmicksScatter(m, 3, 1, 1, 0x13, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 1, 0x15, 2, 8);
        AmapGimmicksScatter(m, 2, 0, 0, 0xB, 1, 8);
        AmapGimmicksScatter(m, 3, 1, 1, 0xC, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 2, 0xD, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 9, 1, m->unk32);
        AmapGimmicksScatter(m, 2, 1, 0, 0xA, 1, m->unk33);
        break;
    case 1:
        AmapGimmicksScatter(m, 2, 1, 0, 9, 1, m->unk32);
        AmapGimmicksScatter(m, 2, 1, 0, 0xA, 1, m->unk33);
        AmapGimmicksScatter(m, 3, 1, 1, 0xB, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xD, 1, 8);
        AmapGimmicksScatter(m, 3, 0, 1, 0x13, 2, 8);
        AmapGimmicksScatter(m, 2, 0, 0, 0xE, 2, 8);
        AmapGimmicksScatter(m, 3, 1, 2, 0x10, 1, 8);
        AmapGimmicksScatter(m, 3, 1, 0, 0x11, 2, 8);
        AmapGimmicksScatter(m, 4, 0, 1, 0x1A, 2, 8);
        break;
    case 2:
        if (m->algo2 == 16) {
            AmapGimmicksScatter(m, 5, 0, 1, 0x17, 1, 8);
            AmapGimmicksScatter(m, 5, 0, 1, 0x1A, 2, 16);
        }
        break;
    case 3:
        AmapGimmicksScatter(m, 2, 1, 0, 9, 1, m->unk32);
        AmapGimmicksScatter(m, 2, 1, 0, 0xA, 1, m->unk33);
        AmapGimmicksScatter(m, 2, 0, 0, 0xB, 1, 8);
        AmapGimmicksScatter(m, 2, 0, 0, 0xE, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xF, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 1, 0x10, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 1, 0x12, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 1, 0x14, 1, 8);
        AmapGimmicksScatter(m, 1, 0, 0, 0x15, 3, 16);
        break;
    case 4:
        AmapGimmicksScatter(m, 2, 1, 0, 9, 1, m->unk32);
        AmapGimmicksScatter(m, 2, 1, 0, 0xA, 1, m->unk33);
        AmapGimmicksScatter(m, 2, 0, 0, 0xB, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xC, 2, 16);
        AmapGimmicksScatter(m, 2, 1, 0, 0xE, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 1, 0x10, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 1, 0x11, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0x12, 1, 8);
        AmapGimmicksScatter(m, 2, 0, 0, 0x1D, 1, m->unk34 / 2);
        AmapGimmicksScatter(m, 2, 1, 2, 0x1E, 1, m->unk34 / 2);
        break;
    case 5:
        AmapGimmicksScatter(m, 2, 1, 0, 9, 1, m->unk32);
        AmapGimmicksScatter(m, 2, 1, 0, 0xA, 1, m->unk33);
        AmapGimmicksScatter(m, 2, 0, 0, 0xB, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xC, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xE, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 1, 0x10, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0x15, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0x16, 1, 8);
        AmapGimmicksScatter(m, 2, 0, 0, 0x1D, 1, m->unk34 / 2);
        AmapGimmicksScatter(m, 2, 1, 0, 0x1E, 1, m->unk34 / 2);
        break;
    case 6:
        AmapGimmicksScatter(m, 2, 1, 0, 9, 1, m->unk32);
        AmapGimmicksScatter(m, 2, 1, 0, 0xA, 1, m->unk33);
        AmapGimmicksScatter(m, 2, 0, 0, 0xB, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xC, 2, 8);
        AmapGimmicksScatter(m, 2, 0, 0, 0xE, 2, 8);
        AmapGimmicksScatter(m, 4, 1, 1, 0x13, 2, 16);
        break;
    case 7:
        AmapGimmicksScatter(m, 2, 1, 0, 9, 1, m->unk32);
        AmapGimmicksScatter(m, 2, 1, 0, 0xA, 1, m->unk33);
        AmapGimmicksScatter(m, 2, 0, 0, 0xB, 1, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xC, 2, 8);
        AmapGimmicksScatter(m, 2, 1, 0, 0xE, 1, 16);
        AmapGimmicksScatter(m, 2, 1, 0, 0xF, 1, 2);
        AmapGimmicksScatter(m, 2, 1, 1, 0xF, 2, 8);
        break;
    }
    if (m->unk116) AmapObstacleGimmicksAdd(m);
    AmapType2WallsDecorate(m);
    AmapGateSwitchesPlace(m);
    AmapSoundSpotsScatter(m);
    AmapStartExitGimmicksAdd(m);
    AmapTilesFixWallGimmicks(m);
    AmapBuildMonsters(m);
    AmapBuildTreasures(m);
    AmapBuildEntrances(m);
    AmapBuildSoundEmitters(m);
    AmapGimmicksSpawn(m);
    AmapBuildPointLights(m);
    AmapBuildCollision(m, m->mapType);
    AmapSpecialRoomsCount(m);
    AmapStartExitMark(m);
    AmapTilesSetAround(m, m->startX, m->startY);
    AmapTilesSetAround(m, m->exitX, m->exitY);
    AmapTileCountsBuild(m);
    AmapLayerClear(3);
    m->result = AmapStartReachCheck(m->exitX, m->exitY, m);
    if (m->result == 0) {
        m->result = AmapPathsHaveRepeat(m);
    }
    v = (f32 *)m->entrances;
    m->startPosX = v[1];
    m->genCount++;
    m->startPosY = v[2];
    m->startPosZ = v[3];
    m->typeFlag = amapTypeFlagTable[m->mapType * 2];
    for (i = 0; i < m->occCount; i++) {
    }
}
