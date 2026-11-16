#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"


s32 SndSeVoiceStop(s32 arg0) {
    SndStop(arg0);
    return 0;
}
