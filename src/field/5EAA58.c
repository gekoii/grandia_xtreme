#include "common.h"
#include "field/menu.h"
#include "protos/field.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);
void ItemCharSlotListBuild(s32);
extern s32 amapItemLogPos;

void ItemWorkInit(void) {
    ItemWork **p = &itemWork;

    *p = FldMemAlloc(sizeof(ItemWork));
    MemSet(*p, 0, sizeof(ItemWork));
    ItemCharSlotListBuild(0);
    ItemUpdateLimits();
    amapItemLogPos = 0;
    ItemSortInventory();
}
