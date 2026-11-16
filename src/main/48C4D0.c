/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

extern s32 _sceTtyInitDone;

void _sceTtyReset(void) {
    _sceTtyInitDone = 0;
}
