#include "common.h"
#include "party.h"

u8 ItemGetUnk9AndName(u16 id, s32 *arg1) {
    PtyItem *d = (PtyItem *)((id << 5) + (u32)ptyItemTable);
    *arg1 = d->name;
    return d->unk9;
}
