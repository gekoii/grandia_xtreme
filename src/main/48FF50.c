/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"

s32 memcmp(void *, void *, s32);
extern u8 _sceLibkernelVersionNo[];

s32 _sceFsVersionMismatch(void) {
    s32 ret;
    u8 *a = _sceLibkernelVersionNo;
    u8 *b = _sceFsIopVersion;

    ret = 0;
    if (memcmp(b, a, 4) != 0 && memcmp(b, _sceFsWildVersion, 4) != 0) {
        ret = memcmp(a, _sceFsWildVersion, 4) != 0;
    }
    return ret;
}
