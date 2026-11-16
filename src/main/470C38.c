/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"

extern char *memcpy(char *, char *, u32);
extern u32 strlen(char *);

void _sceMcChdirApdx(char *arg0) {
    u32 n;

    if (arg0 != NULL) {
        n = strlen((char *)((u32)_sceMcCurDirBuf | 0x20000000)) < 0x400 ? strlen((char *)((u32)_sceMcCurDirBuf | 0x20000000)) : 0x3FF;
        memcpy(arg0, (char *)((u32)_sceMcCurDirBuf | 0x20000000), n);
        arg0[n] = 0;
    }
}
