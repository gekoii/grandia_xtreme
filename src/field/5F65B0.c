/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "save.h"


s32 ItemEggSpellCastField(s32 unused, s32 a_, s16 b, s16 c) {
    s32 sel;
    s16 a;
    s16 k;
    s32 lv;
    s32 r;
    PtyMove *sk;
    PtyMoveEffect *q;
    s8 t;
    s8 tg;
    s32 start;
    s32 end;
    s32 i;
    s32 n;
    s32 hit;
    s32 heal;
    PtyUnit *ch;
    f32 f;

    a = a_;
    if (ptyPartyData->eggs[a].itemId == 0) {
        return -2;
    }
    k = ptyPartyData->eggs[a].spells[b];
    if (k == 0) {
        return -2;
    }
    if (ptyPartyData->eggs[a].owner == -1) {
        return -1;
    }
    sel = PtyPartyGetSlot(ptyPartyData->eggs[a].owner);
    lv = ptyPartyData->eggs[a].mp;
    r = PtyManaEggSpellCost(&ptyPartyUnits[sel], a, b);
    sk = (PtyMove *)((k << 5) + (u32)ptyMoveTable);
    q = sk->levels->effects;
    tg = sk->target;
    t = q->kind;
    if (lv < r) {
        return -3;
    }
    if (!(q->flags & 0x8000) && (ptyPartyUnits[c].status & 0x8000)) {
        return -1;
    }
    start = c;
    end = start + 1;
    if (tg != 1) {
        start = 0;
        end = 4;
    }
    heal = 0;
    hit = 0;
    if (t == 6) {
        if (!(q->flags & 0x9810)) {
            return -1;
        }
        for (i = start; i < end; i++) {
            ch = &ptyPartyUnits[i];
            if (sysSaveData.party[i] != 0) {
                if (q->flags & 0x8000) {
                    if (ch->status & 0x8000) {
                        hit |= 0x8000;
                    }
                }
                if (!(ch->status & 0x8000)) {
                    if (q->flags & 0x800) {
                        if (ch->status & 0x800) {
                            hit |= 0x800;
                        }
                    }
                    if (q->flags & 0x10) {
                        if (ch->status & 0x10) {
                            hit |= 0x10;
                        }
                    }
                    if (q->flags & 0x1000) {
                        if (ch->status & 0x1000) {
                            hit |= 0x1000;
                        }
                    }
                }
            }
        }
    } else {
        for (i = start; i < end; i++) {
            ch = &ptyPartyUnits[i];
            if (ch->id != 0 && !(ch->status & 0x8000) && t == 1) {
                if (ch->hp != ch->maxHp) {
                    heal |= 1;
                }
            }
        }
    }
    if ((hit | heal) == 0) {
        return -1;
    }
    if (hit != 0) {
        for (i = start; i < end; i++) {
            if (sysSaveData.party[i] != 0) {
                ch = &ptyPartyUnits[i];
                if (q->flags & 0x8000) {
                    ch->status &= 0xFFFF7FFF;
                    if (k == 0x46) {
                        ch->hp = ch->maxHp / 2;
                    }
                    if (k == 0x47) {
                        ch->hp = ch->maxHp;
                    }
                }
                if (!(ch->status & 0x8000)) {
                    if (q->flags & 0x800) {
                        ch->status &= 0xFFFFF7FF;
                    }
                    if (q->flags & 0x10) {
                        ch->status &= 0xFFFFFFEF;
                    }
                    if (q->flags & 0x1000) {
                        ch->status &= 0xFFFFEFFF;
                    }
                }
            }
        }
    }
    lv -= r;
    if (heal != 0) {
        {
            PtyUnit *u = (PtyUnit *)(sel * sizeof(PtyUnit) + (u32)ptyPartyUnits);
            f = (f32)(u->params[2] + q->base + u->magicPow[q->element]);
            f *= (f32)q->power * 0.018749999f * u->magicRate[q->element];
        }
        f *= 1.3f;
        f *= (rand() >> 15) * (1.0f / 65536.0f) * 0.2f + 0.9f;
        n = f;
        if (t == 1) {
            for (i = start; i < end; i++) {
                ch = &ptyPartyUnits[i];
                if (ch->id != 0 && !(ch->status & 0x8000)) {
                    ch->hp += n;
                    if (ch->maxHp < ch->hp) {
                        ch->hp = ch->maxHp;
                    }
                }
            }
        }
    }
    ptyPartyData->eggs[a].mp = lv;
    for (i = start; i < end; i++) {
        if (sysSaveData.party[i] != 0) {
            ch = &ptyPartyUnits[i];
            PtyUnitCalcParams(ch, 0, 0);
            PtyCharStoreUnitStats(ch);
        }
    }
    return 0;
}
