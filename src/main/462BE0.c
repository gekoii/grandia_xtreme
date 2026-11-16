#include "common.h"
#include "main/n10.h"
extern s8 sndFindHandleLock;
SndHandle *SndFindHandle(s32 arg0) {
    SndHandle *p; SndHandle *found; s32 i;
    if (sndFindHandleLock != 0) return NULL;
    sndFindHandleLock = 1;
    p = sndHandles; found = NULL;
    for (i = 0; i < 64; i++, p++) {
        if (arg0 == p->id) { if (p->state != 0) { sndFindHandleLock = 0; return p; } }
        else if (p->state != 0) continue;
        if (found == NULL) found = p;
    }
    for (i = 0, p = sndHandles; i < 64; i++, p++) {
        if (arg0 == p->id) { sndFindHandleLock = 0; return p; }
    }
    sndFindHandleLock = 0;
    return found;
}
