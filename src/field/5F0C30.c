/* rodata: 0x00673F08-0x00673F18 */
#include "common.h"
#include "party.h"

void *memset(void *, s32, u32);

u8 ItemGetUseClass(s16 mode, s16 id) {
    u8 tbl[16] = { 0, 2, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    u8 b[16];
    u8 t;
    PtyItem *e;
    PtyMoveEffect *in;
    PtyItemUse *o;
    u8 r;

    memset(b, 0, 16);
    b[4] = 1;
    b[5] = 2;
    b[7] = 1;

    switch (mode) {
    case 0:
        e = (PtyItem *)((id << 5) + (u32)ptyItemTable);
        o = e->use;
        t = e->type;
        if ((u32)(t - 5) >= 6) {
            if (o != NULL && (in = o->effects, in->kind == 6) && !(in->flags & 0x9810)) {
                r = (s8)t == 4 ? 0 : 3;
                break;
            }
        } else {
            r = 1;
            break;
        }
        if ((u32)(id - 201) < 100) {
            r = 2;
        } else {
            r = tbl[(s8)ptyItemTable[id].type];
        }
        break;
    case 1:
        r = tbl[(s8)ptyItemTable[id].type];
        break;
    case 2:
        r = tbl[(s8)ptyItemTable[id].type];
        break;
    case 3:
        in = ptyMoveTable[id].levels->effects;
        r = b[in->kind];
        break;
    default:
        r = 0;
        break;
    }
    return r;
}
