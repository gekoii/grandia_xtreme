#include "common.h"
#include "snd.h"
#include "protos/main.h"

s32 videoDecInputCount(u8 *arg0) {
    return viBufCount(&((VideoDec *)arg0)->vibuf);
}
