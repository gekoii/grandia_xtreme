/* rodata: 0x00674548-0x00674610 */
#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapDbgStatusDraw(AmapWork *m, s32 x, s32 y) {
    s32 i;

    DbgPrintf(x, y, "auto map status");
    DbgPrintf(x + 2, y + 2, "map type   = %d", m->mapType);
    DbgPrintf(x + 2, y + 3, "algorithm1 = %d", m->algo1);
    DbgPrintf(x + 2, y + 4, "algorithm2 = %d", m->algo2);
    DbgPrintf(x + 25, y + 2, "item      = %d", m->itemCount);
    DbgPrintf(x + 25, y + 3, "monster   = %d", m->monsterCount);
    DbgPrintf(x + 25, y + 4, "gimmick   = %d", m->objCount);
    DbgPrintf(x + 25, y + 5, "sroom     = %d", m->specialRooms);
    DbgPrintf(x + 2, y + 6, "template = %d", m->tmplLogCount);
    DbgPrintf(x + 2, y + 7, "   group = ");
    DbgPrintf(x + 2, y + 8, "     new = ");
    DbgPrintf(x + 2, y + 9, "   point = ");
    for (i = 0; i < m->tmplLogCount; i++) {
        DbgPrintf(x + 15 + i * 4, y + 6, "[%2d]", m->tmplLog[i].id);
        DbgPrintf(x + 15 + i * 4, y + 7, "[%2d]", m->tmplLog[i].area);
        DbgPrintf(x + 15 + i * 4, y + 8, "[%2d]", m->tmplLog[i].list);
        DbgPrintf(x + 15 + i * 4, y + 9, "[%2d]", m->tmplLog[i].index);
    }
}
