#include "common.h"
#include "snd.h"

s32 SndIopGetStatus58(void) {
    return (s32)sndIopStatus->unk58;
}
