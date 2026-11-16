#include "common.h"
#include "main/n10.h"

s32 SndFsPopDir(void) {
    if (sndFsDirStackDepth <= 0) {
        return -5;
    }
    sndFsDirStackDepth--;
    sndFsCurDir = sndFsDirStack[sndFsDirStackDepth];
    return 0;
}
