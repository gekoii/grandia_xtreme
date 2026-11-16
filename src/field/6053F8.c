/* rodata: 0x00674418-0x00674428 */
#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void AmapTemplateGetDoor(AmapWork *, s32, s32, s32 *, s32 *);
void AmapTemplateStamp(AmapWork *, s32, s32, s32, s32, s32);

void AmapGenLayoutQuadRooms(AmapWork *arg0) {
    s32 cnt[4] = { 1, 2, 1, 2 };
    s32 x;
    s32 y;
    s32 n;
    s32 bits;
    s32 k;
    s32 i;
    s32 id;
    u8 bx;
    u8 by;

    bits = AmapRandBits(2, 4);
    if (bits & 1) {
        AmapWork *w;

        k = VuRandInt(4);
        n = cnt[k];
        id = AmapTemplatePick(arg0, k, -1);
        AmapBoundsSet(3, 3, arg0->width / 2 - 3, arg0->height / 2 - 3);
        AmapTemplateStamp(arg0, id, -1, -1, -1, -1);
        w = (AmapWork *)((arg0->groupCount - 1) * 42 + (u32)arg0);
        bx = w->groups[0].x;
        by = w->groups[0].y;
        for (i = 0; i < n; i++) {
            AmapTemplateGetDoor(arg0, id, i, &x, &y);
            AmapNodeAdd(bx + x, by + y);
        }
    }
    if ((bits >> 1) & 1) {
        AmapWork *w;

        k = VuRandInt(4);
        n = cnt[k];
        id = AmapTemplatePick(arg0, k, -1);
        AmapBoundsSet(3, arg0->height / 2, arg0->width / 2 - 3, arg0->height - 3);
        AmapTemplateStamp(arg0, id, -1, -1, -1, -1);
        w = (AmapWork *)((arg0->groupCount - 1) * 42 + (u32)arg0);
        bx = w->groups[0].x;
        by = w->groups[0].y;
        for (i = 0; i < n; i++) {
            AmapTemplateGetDoor(arg0, id, i, &x, &y);
            AmapNodeAdd(bx + x, by + y);
        }
    }
    if ((bits >> 2) & 1) {
        AmapWork *w;

        k = VuRandInt(4);
        n = cnt[k];
        id = AmapTemplatePick(arg0, k, -1);
        AmapBoundsSet(arg0->width / 2 + 3, 3, arg0->width - 3, arg0->height / 2 - 3);
        AmapTemplateStamp(arg0, id, -1, -1, -1, -1);
        w = (AmapWork *)((arg0->groupCount - 1) * 42 + (u32)arg0);
        bx = w->groups[0].x;
        by = w->groups[0].y;
        for (i = 0; i < n; i++) {
            AmapTemplateGetDoor(arg0, id, i, &x, &y);
            AmapNodeAdd(bx + x, by + y);
        }
    }
    if ((bits >> 3) & 1) {
        AmapWork *w;

        k = VuRandInt(4);
        n = cnt[k];
        id = AmapTemplatePick(arg0, k, -1);
        AmapBoundsSet(arg0->width / 2 + 3, arg0->height / 2, arg0->width - 3, arg0->height - 3);
        AmapTemplateStamp(arg0, id, -1, -1, -1, -1);
        w = (AmapWork *)((arg0->groupCount - 1) * 42 + (u32)arg0);
        bx = w->groups[0].x;
        by = w->groups[0].y;
        for (i = 0; i < n; i++) {
            AmapTemplateGetDoor(arg0, id, i, &x, &y);
            AmapNodeAdd(bx + x, by + y);
        }
    }
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
