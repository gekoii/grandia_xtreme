#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

void GsDmaInit(void) {
    gsDmaEnv.sts = 0;
    gsDmaEnv.tqwc = 1;
    gsDmaEnv.std = 0;
    gsDmaEnv.mfd = 0;
    gsDmaEnv.rcycle = 0;
    gsDmaEnv.express = 0;
    gsDmaEnv.notify = 0;
    gsDmaEnv.sqwc = 0;
    gsDmaEnv.rbadr = NULL;
    gsDmaEnv.rbmsk = 0;
    sceDmaPutEnv(&gsDmaEnv);
    /* calls through a cast: a direct call keeps the five %hi(gsDmaChan*) in s registers */
    gsDmaChanVif0 = ((sceDmaChan *(*)(u32))sceDmaGetChan)(0);
    gsDmaChanVif1 = ((sceDmaChan *(*)(u32))sceDmaGetChan)(1);
    gsDmaChanFromSpr = ((sceDmaChan *(*)(u32))sceDmaGetChan)(8);
    gsDmaChanToSpr = ((sceDmaChan *(*)(u32))sceDmaGetChan)(9);
    gsDmaChanGif = ((sceDmaChan *(*)(u32))sceDmaGetChan)(2);
}
