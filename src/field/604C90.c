#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void AmapTemplateGetDoor(AmapWork *, s32, s32, s32 *, s32 *);
void AmapTemplateStamp(AmapWork *, s32, s32, s32, s32, s32);

void AmapGenLayoutRoom3Door(AmapWork *arg0, s16 arg1) {
    s32 x;
    s32 y;
    AmapWork *w;
    u8 bx;
    u8 by;

    AmapBoundsSet(6, 6, arg0->width - 6, arg0->height - 6);
    AmapTemplateStamp(arg0, arg1, -1, -1, -1, -1);
    AmapBoundsSet(2, 2, arg0->width - 2, arg0->height - 2);
    AmapStartPlace(arg0);
    AmapExitPlace(arg0);
    w = (AmapWork *)((arg0->groupCount - 1) * 42 + (u32)arg0);
    bx = w->groups[0].x;
    by = w->groups[0].y;
    AmapTemplateGetDoor(arg0, arg1, 0, &x, &y);
    AmapNodeAdd(bx + x, by + y);
    AmapTemplateGetDoor(arg0, arg1, 1, &x, &y);
    AmapNodeAdd(bx + x, by + y);
    AmapTemplateGetDoor(arg0, arg1, 2, &x, &y);
    AmapNodeAdd(bx + x, by + y);
    AmapNodesScatter(arg0->nodeCount);
    AmapBoundsSet(-1, -1, -1, -1);
    AmapLayerClear(3);
    AmapNodesLinkNearest(arg0, 4);
    AmapNodesLinkGroups(arg0, 4);
    AmapNodesMark();
}
