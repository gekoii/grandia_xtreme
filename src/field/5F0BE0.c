#include "common.h"
#include "party.h"

s16 PtyCharGetEquip(s16 arg0, s16 arg1) {
    return ptyCharData[arg0].equip[arg1];
}
