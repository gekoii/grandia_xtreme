#include "common.h"
#include "main/n10.h"

s32 SndFsPushDir(void) {
    if (sndFsDirStackDepth < 7) {
        sndFsDirStack[sndFsDirStackDepth++] = sndFsCurDir;
        return 0;
    }
    return -5;
}
