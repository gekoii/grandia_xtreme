#include "common.h"

typedef struct {
    s32 x0;
    s32 x4;
    u8 pad8;
    s8 x9;
    u8 padA[0x24 - 0xA];
} Ent;

/* Local view of mcReqPool (McReqPool, main/n08.h): 0x24-byte records from the
 * pool start; with &slots[i] gcc adds the +4 to the base (see n08.h). */
extern Ent mcReqPool[];

s32 McReqIsDone(s32 i) {
    Ent *e;

    if (i < 0 || mcReqPool[0].x0 < i) {
        return 0;
    }
    if (mcReqPool[i].x4 == 0) {
        return 0;
    }
    e = &mcReqPool[i];
    if (e->x9 == 2) {
        return 1;
    }
    if (e->x9 < 2) {
        return 0;
    }
    if (e->x9 == -1) {
        return -1;
    }
    return 0;
}
