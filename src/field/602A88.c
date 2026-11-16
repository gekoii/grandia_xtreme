#include "common.h"
#include "field/n21.h"

void AmapGenReset(AmapWork *arg0) {
    arg0->unkC = 0;
    arg0->algo1 = 0;
    arg0->startFacing = 0;
    arg0->width = 0x1C;
    arg0->height = 0x1C;
    arg0->bufSize = 0;
    arg0->cellBufSize = 0;
    arg0->tiles = 0;
    arg0->cells = 0;
    arg0->result = 0;
    arg0->unk384 = 0;
}
