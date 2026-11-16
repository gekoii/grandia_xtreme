#include "common.h"
#include "protos/main.h"
#include "snd.h"


void videoDecBeginPut(u8 *arg0, u32 *arg1, u32 *arg2, u32 *arg3, u32 *arg4) {
    viBufBeginPut(&((VideoDec *)arg0)->vibuf, (u8 **)arg1, (s32 *)arg2, (u8 **)arg3, (s32 *)arg4);
}
