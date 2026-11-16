#include "common.h"
#include "party.h"

s8 ItemGetRank(u16 id) {
    return ptyItemTable[id].rank;
}
