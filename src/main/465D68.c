#include "common.h"
#include "protos/main.h"
#include "snd.h"


void videoDecEndPut(u8 *arg0, s32 arg1) {
    viBufEndPut(&((VideoDec *)arg0)->vibuf, arg1);
}
