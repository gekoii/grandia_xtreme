#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void AmapTemplateGetDoor(AmapWork *, s32, s32, s32 *, s32 *);
void AmapTemplateStamp(AmapWork *, s32, s32, s32, s32, s32);

#define ROOM_POS()                                                  \
    {                                                               \
        AmapWork *e = (AmapWork *)((u8 *)m + (m->groupCount - 1) * sz);              \
        ex = e->groups[0].x;                                                  \
        ey = e->groups[0].y;                                                  \
    }

#define REFRESH()          \
    AmapLayerClear(3);      \
    AmapNodesLinkNearest(m, 8);   \
    AmapNodesLinkGroups(m, 8);   \
    AmapNodesMark();       \
    *p = 0;

void AmapGenLayoutFixedB(AmapWork *m) {
    s32 dx;
    s32 dy;
    s32 sz;
    s32 *p;
    s32 ex;
    s32 ey;

    p = &amapNodeCount;
    sz = 0x2A;
    {
        s32 room;
        room = AmapTemplatePick(m, 9, 0);
        AmapTemplateStamp(m, room, 2, 0x15, 1, 0);
        ROOM_POS();
        AmapTemplateGetDoor(m, room, 0, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
    }

    {
        s32 room;
        room = AmapTemplatePick(m, 9, 3);
        AmapTemplateStamp(m, room, 5, 0xC, 2, 0);
        ROOM_POS();
        AmapTemplateGetDoor(m, room, 1, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
        REFRESH();
        AmapTemplateGetDoor(m, room, 0, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
    }

    {
        s32 room;
        room = AmapTemplatePick(m, 9, 5);
        AmapTemplateStamp(m, room, 3, 1, 1, 0);
        ROOM_POS();
        AmapTemplateGetDoor(m, room, 1, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
        REFRESH();
        AmapTemplateGetDoor(m, room, 0, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
    }

    {
        s32 room;
        room = AmapTemplatePick(m, 9, 6);
        AmapTemplateStamp(m, room, 0x12, 1, 1, 0);
        ROOM_POS();
        AmapTemplateGetDoor(m, room, 0, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
        REFRESH();
        AmapTemplateGetDoor(m, room, 1, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
    }

    {
        s32 room;
        room = AmapTemplatePick(m, 9, 3);
        AmapTemplateStamp(m, room, 0x12, 0xB, 2, 0);
        ROOM_POS();
        AmapTemplateGetDoor(m, room, 0, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
        REFRESH();
        AmapTemplateGetDoor(m, room, 1, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
    }

    {
        s32 room;
        room = AmapTemplatePick(m, 9, 1);
        AmapTemplateStamp(m, room, 0x14, 0x15, 1, 0);
        ROOM_POS();
        AmapTemplateGetDoor(m, room, 0, &dx, &dy);
        AmapNodeAdd(ex + dx, ey + dy);
    }

    AmapBoundsSet(2, 2, m->width - 2, m->height - 2);
    AmapStartPlace(m);
    AmapExitPlace(m);
    AmapBoundsSet(-1, -1, -1, -1);
    AmapNodesLinkNearest(m, 8);
    AmapNodesLinkGroups(m, 8);
    AmapNodesMark();
    AmapFloorFixGaps(m);
    AmapVoidFillAll(m);
}
