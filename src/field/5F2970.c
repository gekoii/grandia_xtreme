#include "common.h"
#include "party.h"

s8 ItemGetPickupModel(u16 id) {
    return ptyItemTable[id].model;
}
