#include "common.h"
#include "main/n05.h"

f32 *GphEnvMapGetColor(s32 arg0) {
    return gphEnvMapTable[arg0].color;
}
