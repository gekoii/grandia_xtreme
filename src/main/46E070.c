/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libdma.h"

extern sceDmaChan *_sceDmaChanTable[];

sceDmaChan *sceDmaGetChan(u32 arg0) {
    if (arg0 < 10) {
        return _sceDmaChanTable[arg0];
    }
    return NULL;
}
