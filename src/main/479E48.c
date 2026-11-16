/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"



void SyncDCache(void *, void *);

ScePadData *_scePadGetDmaStr(s32 arg0, s32 arg1) {
    ScePadData *p = _scePadPortTable[arg0][arg1].padArea;
    s32 i;
    SyncDCache(p, p + 2);
    i = p[0].frame < p[1].frame;
    return &p[i];
}
