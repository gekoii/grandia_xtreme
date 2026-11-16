/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"

s32 DIntr(void);
void EIntr(void);

s32 sceCdCallback(s32 arg0) {
    s32 old;

    if (sceCdSync(1) != 0) {
        return 0;
    }
    DIntr();
    old = (s32)sceCdCbfunc;
    sceCdCbfunc = (void (*)(s32))arg0;
    EIntr();
    return old;
}
