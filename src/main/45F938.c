#include "common.h"
#include "main/n10.h"

extern s8 sndFsNameBuf[];

s32 strcmp(s8 *, s8 *);

s32 SndFsFind(u8 *path, s32 *out) {
    SndFsDirEntry *e;
    s32 i;
    s32 isDir;
    u8 *src;

    if (out == NULL) {
        return -12;
    }
    if (path == NULL) {
        return -1;
    }
    /* goto loop (next:) inside a do-while(0): a for (;;) loop gets the loop
     * pass (bigger frame, 13 more insns) */
    do {
        src = path;
        e = sndFsRootDir;
    next:
        isDir = 0;
        for (i = 0; (sndFsNameBuf[i] = *src++) != 0; ) {
            if (sndFsNameBuf[i] == '/' || sndFsNameBuf[i] == '\\') {
                sndFsNameBuf[i] = 0;
                isDir = 1;
                break;
            }
            i++;
            if (i == 16) {
                goto fail;
            }
        }
        while (e->lsn > 0 && strcmp(e->name, sndFsNameBuf) != 0) {
            e++;
            if (e->lsn == -1) {
                return -1;
            }
        }
        if (e->flags != 2 && isDir == 0) {
            out[0] = e->lsn;
            return out[1] = e->size;
        }
    } while (0);
    e = e->child;
    goto next;
fail:
    return -1;
}
