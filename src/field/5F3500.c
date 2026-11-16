#include "common.h"
#include "party.h"

s32 ItemGetEquipCategory(s16 id) {
    PtyItem *d = (PtyItem *)((id << 5) + (u32)ptyItemTable);
    PtyEquipData *e = d->equip;
    s8 t;

    if (e == NULL) {
        return 0;
    }
    t = d->type;
    if (t == 5) {
        return 1;
    }
    if (t == 6) {
        return 2;
    }
    if (t == 7) {
        return 2;
    }
    if (t == 8) {
        return 2;
    }
    if (t == 9) {
        return 6;
    }
    if (e->stats[0] != 0) {
        return 1;
    }
    if (e->stats[1] != 0) {
        return 2;
    }
    if (e->stats[5] != 0) {
        return 6;
    }
    return 0;
}
