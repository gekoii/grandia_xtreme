#include "common.h"
#include "field/state.h"

/* The motion table starts at +0x10 (state.h: pad10, then motions). */
s32 FldSpotGetMotionList(void) {
    return (s32)fldSpotWork + 0x10;
}
