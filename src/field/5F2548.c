#include "common.h"
#include "field/n17.h"

u8 FldChrPackGetMapIcon(s16 arg0) {
    return ((u8 *)fldChrPackTable[arg0].pack->enemyData)[2];
}
