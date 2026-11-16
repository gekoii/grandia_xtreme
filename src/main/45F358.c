#include "common.h"
#include "main/n10.h"

s32 SndIopIsCmdDmaDone(s32 arg0) {
    if (sndIopCmdDmaBusy != 0) {
        if (sceSifDmaStat(sndIopCmdDmaId) >= 0) {
            return 0;
        }
    }
    return 1;
}
