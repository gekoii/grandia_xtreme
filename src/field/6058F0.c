/* rodata: 0x00674428-0x00674430 */
#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

void AmapTemplateGetDoor(AmapWork *, s32, s32, s32 *, s32 *);
void AmapTemplateStamp(AmapWork *, s32, s32, s32, s32, s32);

void AmapGenLayoutQuadRoomsMedium(AmapWork *m) {
    s32 sizes[2] = { 1, 2 };
    s32 dx;
    s32 dy;
    s32 cnt;
    s32 mask;
    s32 room;
    s32 r;
    s32 i;
    u8 ex;
    u8 ey;

    mask = AmapRandBits(2, 4);
    if (mask & 1) {
        r = VuRandInt(2);
        cnt = sizes[r];
        room = AmapTemplatePick(m, r + 2, -1);
        AmapBoundsSet(3, 3, (m->width / 2) - 3, (m->height / 2) - 3);
        AmapTemplateStamp(m, room, -1, -1, -1, -1);
        {
            AmapWork *e = (AmapWork *)((u8 *)m + (m->groupCount - 1) * 0x2A);
            ex = e->groups[0].x;
            ey = e->groups[0].y;
        }
        for (i = 0; i < cnt; i++) {
            AmapTemplateGetDoor(m, room, i, &dx, &dy);
            AmapNodeAdd(ex + dx, ey + dy);
        }
    }
    if ((mask >> 1) & 1) {
        r = VuRandInt(2);
        cnt = sizes[r];
        room = AmapTemplatePick(m, r + 2, -1);
        AmapBoundsSet(3, m->height / 2, (m->width / 2) - 3, m->height - 3);
        AmapTemplateStamp(m, room, -1, -1, -1, -1);
        {
            AmapWork *e = (AmapWork *)((u8 *)m + (m->groupCount - 1) * 0x2A);
            ex = e->groups[0].x;
            ey = e->groups[0].y;
        }
        for (i = 0; i < cnt; i++) {
            AmapTemplateGetDoor(m, room, i, &dx, &dy);
            AmapNodeAdd(ex + dx, ey + dy);
        }
    }
    if ((mask >> 2) & 1) {
        r = VuRandInt(2);
        cnt = sizes[r];
        room = AmapTemplatePick(m, r + 2, -1);
        AmapBoundsSet((m->width / 2) + 3, 3, m->width - 3, (m->height / 2) - 3);
        AmapTemplateStamp(m, room, -1, -1, -1, -1);
        {
            AmapWork *e = (AmapWork *)((u8 *)m + (m->groupCount - 1) * 0x2A);
            ex = e->groups[0].x;
            ey = e->groups[0].y;
        }
        for (i = 0; i < cnt; i++) {
            AmapTemplateGetDoor(m, room, i, &dx, &dy);
            AmapNodeAdd(ex + dx, ey + dy);
        }
    }
    if ((mask >> 3) & 1) {
        r = VuRandInt(2);
        cnt = sizes[r];
        room = AmapTemplatePick(m, r + 2, -1);
        AmapBoundsSet((m->width / 2) + 3, m->height / 2, m->width - 3, m->height - 3);
        AmapTemplateStamp(m, room, -1, -1, -1, -1);
        {
            AmapWork *e = (AmapWork *)((u8 *)m + (m->groupCount - 1) * 0x2A);
            ex = e->groups[0].x;
            ey = e->groups[0].y;
        }
        for (i = 0; i < cnt; i++) {
            AmapTemplateGetDoor(m, room, i, &dx, &dy);
            AmapNodeAdd(ex + dx, ey + dy);
        }
    }
    AmapBoundsSet(2, 2, m->width - 2, m->height - 2);
    AmapStartPlace(m);
    AmapExitPlace(m);
    AmapNodesScatter(m->nodeCount);
    AmapBoundsSet(-1, -1, -1, -1);
    AmapLayerClear(3);
    AmapNodesLinkNearest(m, 4);
    AmapNodesLinkGroups(m, 4);
    AmapNodesMark();
}
