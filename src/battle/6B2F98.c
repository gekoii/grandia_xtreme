/* rodata: 0x006E1608-0x006E1660 */
#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void MgcSeLoadSkill(void) {
    char buf[256];
    MgcWork *m = &mgcWork;
    s16 id = m->id;
    BtlUnit *c;
    s32 k;
    s32 se;

    c = m->caster;
    if (id < 91) {
        return;
    }
    if (c == NULL && id >= 201 && id % 10 == 2) {
        return;
    }
    k = c->def->id;
    if (c->id < 4) {
        if (id >= 221) {
            sprintf(buf, "MUSIC/W%.3d.SE", id);
            if (id < 231) {
                se = id + 0x193B;
            } else if (id < 241) {
                se = id + 0x1939;
            } else if (id < 261) {
                se = id + 0x1936;
            } else if (id <= 280) {
                se = id + 0x1932;
            } else {
                se = id + 0x192A;
            }
        } else {
            sprintf(buf, "MUSIC/W%.3d%s.SE", id, m->unk14 ? "B" : "A");
            se = ((id - 101) / 6 * 5 + (id - 101) % 6) * 2 + 0x19C8 + m->unk14;
        }
    } else {
        if ((k & 0xFF) > 80) {
            k &= 0xFF;
        }
        if (id >= 91) {
            if (id < 95) {
                return;
            }
            if (id == 95) {
                if (k == 0xD1) {
                    sprintf(buf, "MUSIC/QD.SE");
                    se = 0xB54;
                } else {
                    sprintf(buf, "MUSIC/BD.SE");
                    se = 0x1766;
                }
                goto play; /* goto: an if/else around the generic case changes the allocation */
            }
        }
        if (id >= 148) {
            return;
        }
        sprintf(buf, "MUSIC/W%.3d%s.SE", id, m->unk14 ? "B" : "A");
        se = ((id - 101) / 6 * 5 + (id - 101) % 6) * 2 + 0x19C8 + m->unk14;
    }
play:
    if (se > 0) {
        BtlStrSpaceToZero(buf);
        SndSeBankLoadStart(buf, 0, se, 0);
    }
}
