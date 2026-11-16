#include "common.h"
#include "party.h"

s8 ItemGetEquipModelId(s16 arg0) {
    return ptyItemTable[arg0].equip->unkF;
}
