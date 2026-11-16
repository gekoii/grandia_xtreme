/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern s32 _sceTtyInitDone;

s32 read(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        if (_sceTtyInitDone == 0) {
            if (sceTtyInit() == 0) {
                goto fail;
            }
            _sceTtyInitDone = 1;
        }
        return sceTtyRead(arg1, arg2);
    }
fail:
    return -1;
}
