/* rodata: 0x00673DD8-0x00673DE8 */
#include "common.h"
#include "field/menu.h"
#include "party.h"
#include "protos/field.h"

void ItemInfoFill(ItemBagRec *arg0, u16 arg1, s16 arg2) {
    s8 tbl[16] = { 0, 1, 1, 1, 1, 2, 3, 3, 3, 3, 0, 0, 0, 1, 0, 0 };
    PtyItem *e;
    s32 v4;
    PtyItemUse *sub;
    s8 *p = NULL;

    s32 c;
    u8 u1;
    s8 t;

    e = (PtyItem *)((arg1 << 5) + (u32)ptyItemTable);
    u1 = e->icon;
    c = e->name;
    v4 = e->price;
    sub = e->use;
    arg0->unk2 = u1;
    arg0->unkC = c;
    arg0->unk10 = e;
    arg0->unkA = arg2;
    arg0->id = arg1;
    arg0->unk8 = ItemGetListCategory(0, arg1);
    t = tbl[((PtyItem *)((arg1 << 5) + (u32)ptyItemTable))->type];
    arg0->unk7 = t;
    arg0->unk6 = ItemGetUseClass(0, arg1);
    arg0->unk14 = v4;
    arg0->unk5 = 0;
    arg0->unk4 = 0;
    if (sub != NULL) {
        arg0->unk5 = sub->target;
        p = (s8 *)sub->effects;
    }
    if (p != NULL && *p > 0 && (*p < 4 || *p == 6)) {
        arg0->unk4 = 1;
    }
}
