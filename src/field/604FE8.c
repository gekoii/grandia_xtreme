/* rodata: 0x006743F8-0x00674418 */
#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void AmapTemplateGetDoor(AmapWork *, s32, s32, s32 *, s32 *);
void AmapTemplateStamp(AmapWork *, s32, s32, s32, s32, s32);

void AmapGenLayoutSplit(AmapWork *arg0, s16 arg1) {
    s32 x;
    s32 y;
    AmapWork *w;
    u8 bx;
    u8 by;
    s32 left;
    s32 l2;
    s32 a;
    s32 b;

    AmapBoundsSet(6, 6, arg0->width - 6, arg0->height - 6);
    AmapTemplateStamp(arg0, arg1, -1, -1, -1, -1);
    w = (AmapWork *)((arg0->groupCount - 1) * 42 + (u32)arg0);
    bx = w->groups[0].x;
    by = w->groups[0].y;
    if (VuRandInt(100) + 1 < 50) {
        AmapTemplateGetDoor(arg0, arg1, 0, &x, &y);
        left = bx + x;
        AmapNodeAdd(left, by + y);
        l2 = left + 2;
        AmapBoundsSet(2, 2, l2, arg0->height - 2);
        AmapStartPlace(arg0);
        a = arg0->nodeCount * 0.3;
        b = arg0->nodeCount * 0.7;
        AmapNodesScatter(a);
        AmapBoundsSet(-1, -1, -1, -1);
        AmapLayerClear(3);
        AmapNodesLinkNearest(arg0, 4);
        AmapNodesLinkGroups(arg0, 4);
        AmapNodesMark();
        AmapCorridorBlockAround(arg0);
        amapNodeCount = 0;
        AmapTemplateGetDoor(arg0, arg1, 1, &x, &y);
        AmapNodeAdd(bx + x, by + y);
        AmapBoundsSet(l2, 2, arg0->width - 2, arg0->height - 2);
        AmapExitPlace(arg0);
        AmapNodesScatter(b);
        AmapBoundsSet(-1, -1, -1, -1);
        AmapTemplateGetDoor(arg0, arg1, 2, &x, &y);
        AmapNodeAdd(bx + x, by + y);
    } else {
        AmapTemplateGetDoor(arg0, arg1, 0, &x, &y);
        AmapNodeAdd(bx + x, by + y);
        AmapTemplateGetDoor(arg0, arg1, 2, &x, &y);
        left = bx + x;
        AmapNodeAdd(left, by + y);
        l2 = left + 2;
        AmapBoundsSet(2, 2, l2, arg0->height - 2);
        AmapStartPlace(arg0);
        a = arg0->nodeCount * 0.3;
        b = arg0->nodeCount * 0.7;
        AmapNodesScatter(a);
        AmapBoundsSet(-1, -1, -1, -1);
        AmapLayerClear(3);
        AmapNodesLinkNearest(arg0, 4);
        AmapNodesLinkGroups(arg0, 4);
        AmapNodesMark();
        AmapCorridorBlockAround(arg0);
        amapNodeCount = 0;
        AmapTemplateGetDoor(arg0, arg1, 1, &x, &y);
        AmapNodeAdd(bx + x, by + y);
        AmapBoundsSet(l2, 2, arg0->width - 2, arg0->height - 2);
        AmapExitPlace(arg0);
        AmapNodesScatter(b);
        AmapBoundsSet(-1, -1, -1, -1);
    }
    AmapLayerClear(3);
    AmapNodesLinkNearest(arg0, 4);
    AmapNodesLinkGroups(arg0, 4);
    AmapNodesMark();
}
