/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void _SyncDCache(u32, u32);

void iSyncDCache(u32 arg0, u32 arg1) {
    _SyncDCache(arg0 & 0xFFFFFFC0, arg1 & 0xFFFFFFC0);
}
