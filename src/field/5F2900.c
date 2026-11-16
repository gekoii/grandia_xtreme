#include "common.h"
#include "party.h"

s32 ItemGetIconAndName(u16 id, s16 *arg1, s32 *arg2) {
    PtyItem *d = (PtyItem *)((id << 5) + (u32)ptyItemTable);
    *arg1 = d->icon;
    *arg2 = d->name;
    return 0;
}
