#include "common.h"
#include "main/n08.h"

void sceDmaSend(sceDmaChan *, s32);

void GsFromSprDmaSend(s32 arg0) {
    sceDmaSend(gsDmaChanFromSpr, arg0);
}
