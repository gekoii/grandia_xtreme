#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void AmapTemplateGetDoor(AmapWork *, s32, s32, s32 *, s32 *);
void AmapTemplateStamp(AmapWork *, s32, s32, s32, s32, s32);

void AmapGenLayoutRoom1Door(AmapWork *arg0, s16 arg1) {
    s32 x;
    s32 y;
    AmapWork *w;

    AmapBoundsSet(3, 3, arg0->width - 3, arg0->height - 3);
    AmapTemplateStamp(arg0, arg1, -1, -1, -1, -1);
    AmapTemplateGetDoor(arg0, arg1, 0, &x, &y);
    w = (AmapWork *)((arg0->groupCount - 1) * 42 + (u32)arg0);
    AmapNodeAdd(w->groups[0].x + x, w->groups[0].y + y);
    AmapBoundsSet(2, 2, arg0->width - 2, arg0->height - 2);
    AmapStartPlace(arg0);
    AmapExitPlace(arg0);
    AmapNodesScatter(arg0->nodeCount);
    AmapBoundsSet(-1, -1, -1, -1);
    AmapLayerClear(3);
    AmapNodesLinkNearest(arg0, 4);
    AmapNodesLinkGroups(arg0, 4);
    AmapNodesMark();
}
