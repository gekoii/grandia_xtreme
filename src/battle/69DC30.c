/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"
#include "gph.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))


void BtlCmdMenuOpen(BtlUnit *u) {
    BtlCmdMenu *m = &btlCmdMenu;
    BtlUnit *p;
    BtlUnit *p1;
    s32 j;
    GphPart *o;
    s32 b;
    GphPart *o2;
    GphPart *o3;
    PtyCombo *e;
    s32 *q;
    s32 i;
    BtlUnit *pb;
    s32 lv;
    s32 d;
    f32 hp;
    f32 r;
    f32 one;
    f32 k;
    f32 sc;
    s32 t;
    PtyUnit *mdl;

    m->unk9 = 0;
    m->sel = 0;
    m->unk8 = 0;
    m->unkB = 0;
    btlCmdMenu.state = 0x10;
    m->unit = u;
    m->flags = u->params->cmdDisable;
    for (j = 0, p1 = &btlUnits[4]; j < 12; j++, p1++) {
        if (p1->state >= 0x40 && p1->def->data->unk7 >= 100) {
            m->flags |= 4;
        }
    }
    if (u->params->status & 0x402) {
        m->flags |= 0x40;
    }
    if (u->params->status & 0x200) {
        m->flags |= 0x80;
    }
    for (b = 0; b < 8; b++) {
        if ((m->flags >> b) & 1) {
            BtlChrFindPart(btlCmdMenuRingInst, b + 0x65)->flags &= ~8;
        } else {
            BtlChrFindPart(btlCmdMenuRingInst, b + 0x65)->flags |= 8;
        }
    }
    if (BtlUnitCountManaEggs(u) == 0) {
        o = BtlChrFindPart(btlCmdMenuRingInst, 7);
        o->flags |= 8;
        (o - 1)->flags |= 8;
        o2 = BtlChrFindPart(btlCmdMenuRingInst, 0x6B);
        o2->flags |= 8;
    } else {
        o3 = BtlChrFindPart(btlCmdMenuRingInst, 7);
        o3->flags &= ~8;
    }
    lv = u->params->level;
    q = m->hidden;
    for (i = 0; i < 4; i++) {
        *q++ = 0;
    }
    pb = btlUnits;
    one = 1.0f;
    k = 0.04f;
    sc = 1.0f / 65536.0f;
    p = pb + 4;
    t = 1;
    for (; i < 16; i++, p++, q++) {
        *q = 0;
        hp = u->params->mcActDisp;
        if (hp < one && p->state >= 0x40) {
            d = p->params->level;
            if (lv < d) {
                r = (f32)(d - lv) * k * (one - hp);
                if ((rand() >> 15) * sc <= r) {
                    *q = t;
                }
            }
        }
    }
    m->enabledMask = 0;
    for (i = 0, e = ptyComboTable; i < 50; i++, e++) {
        if (e->moveId > 0 && ptyPartyData->moveLevels[i + 40].level == 0) {
            m->enabledMask |= BtlCmdComboGetMembers(u, e, 1, 0);
        }
    }
    m->enabledMask &= ~(1 << u->id);
    if (m->enabledMask != 0) {
        mdl = u->params;
        if (mdl->combiHint > 0.0f && RND01() <= mdl->combiHint) {
            IpComboMarkSet(1);
            FxComboLinkSet(u, m->enabledMask);
        }
    }
    m->area.active = 0;
    IpGaugeSetActor(u->id);
    BtlCmdMenuSetState(16);
    IpTransStart(0, -0.041666668f);
}
