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

s32 McReqGetStatus(s32 i) {
    Ent *e;
    s32 buf[2]; /* unused: gives the 16-byte frame */

    if (i < 0 || mcReqPool[0].x0 < i) {
    err: /* error return first in the layout; the later test jumps back here */
        return 0;
    }
    if (mcReqPool[i].x4 == 0) {
        goto err;
    }
    e = &mcReqPool[i];
    if (e->x9 == -1) return -1;
    if (e->x9 == 2) return 1;
    /* redundant test: keeps the final return 0 from being turned into a
       conditional move or cross-jumped into the early return 0 */
    if (e->x9 != 2) return 0;
    return 0;
}
