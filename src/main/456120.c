#include "common.h"
#include "main/n08.h"

void sceDmaSend(sceDmaChan *, s32);

void GsToSprDmaSend(s32 arg0) {
    sceDmaSend(gsDmaChanToSpr, arg0);
}
