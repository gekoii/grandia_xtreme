/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"

s32 memcmp(void *, void *, s32);
extern u8 _sceLibkernelVersionNo[];
extern u8 *_sceSifLoadWildVersion;

s32 _sceSifLoadFileVersionMismatch(void) {
    u8 *a;
    u8 *b;
    s32 ret;

    a = _sceLibkernelVersionNo;
    b = _sceSifLoadIopVersion;
    ret = 0;
    if (memcmp(b, a, 4) != 0 && memcmp(b, _sceSifLoadWildVersion, 4) != 0) {
        ret = memcmp(a, _sceSifLoadWildVersion, 4) != 0;
    }
    return ret;
}
