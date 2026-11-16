/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"

extern s32 _sceTtyInitDone;

s32 write(s32 arg0, s32 arg1, s32 arg2) {
    if ((u32) (arg0 - 1) < 2U) {
        if (_sceTtyInitDone == 0) {
            if (sceTtyInit() != 0) {
                _sceTtyInitDone = 1;
                goto block_4;
            }
            goto block_5;
        }
block_4:
        return sceTtyWrite(arg1, arg2);
    }
block_5:
    return -1;
}
