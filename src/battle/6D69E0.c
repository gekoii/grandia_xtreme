/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/misc.h"

typedef struct Stats {
    u8 pad0[8];
    s8 unk8[2];         /* 0x08 */
    u8 padA[0x3E];
    s32 flags;          /* 0x48 */
    u8 pad4C[4];
    s32 unk50;          /* 0x50 */
    s32 unk54;          /* 0x54 */
    s32 unk58;          /* 0x58 */
    s32 unk5C;          /* 0x5C */
    u8 pad60[0xC];
    f32 unk6C;          /* 0x6C */
    u8 pad70[0xC];
    f32 unk7C[13];      /* 0x7C */
    f32 unkB0[13];      /* 0xB0 */
    s32 unkE4[13];      /* 0xE4 */
    f32 unk118[9];      /* 0x118 */
    s32 unk13C[9];      /* 0x13C */
    f32 unk160[3];      /* 0x160 */
    u8 pad16C;
    u8 unk16D;          /* 0x16D */
    s8 unk16E;          /* 0x16E */
    u8 pad16F[0xD];
    s32 unk17C;         /* 0x17C */
    u8 pad180[0x10];
    s32 unk190;         /* 0x190 */
    u8 pad194[0x10];
    s32 unk1A4;         /* 0x1A4 */
    u8 pad1A8[0x24];
    f32 unk1CC;         /* 0x1CC */
    u8 pad1D0[0x24];
    f32 unk1F4;         /* 0x1F4 */
    u8 pad1F8[0x54];
    s32 unk24C;         /* 0x24C */
    f32 unk250[9];      /* 0x250 */
    f32 unk274;         /* 0x274 */
    f32 unk278[12];     /* 0x278 */
    f32 unk2A8;         /* 0x2A8 */
    f32 unk2AC;         /* 0x2AC */
} Stats;

typedef struct Unit {
    s32 id;             /* 0x00 */
    u8 pad4[0x74];
    s32 unk78;          /* 0x78 */
    u8 pad7C[0x136];
    union { u16 w; } unk1B2;         /* 0x1B2 */
    u8 pad1B4[0x3C];
    Stats *stats;       /* 0x1F0 */
    u8 pad1F4[0xC];
} Unit;

typedef struct Skill {
    s8 kind;            /* 0x00 */
    u8 unk1;            /* 0x01 */
    s16 unk2;           /* 0x02 */
    s16 unk4;           /* 0x04 */
    s16 unk6;           /* 0x06 */
    s16 unk8;           /* 0x08 */
    s16 unkA;           /* 0x0A */
    s16 unkC;           /* 0x0C */
    s16 unkE;           /* 0x0E */
    s32 unk10;          /* 0x10 */
    s8 unk14;           /* 0x14 */
    s8 unk15;           /* 0x15 */
    u8 unk16;           /* 0x16 */
    u8 unk17;           /* 0x17 */
} Skill;

typedef struct Hit {
    Unit *unit;         /* 0x00 */
    Skill *skill;       /* 0x04 */
    s32 unk8;           /* 0x08 */
    s32 flags;          /* 0x0C */
    s32 target;         /* 0x10 */
    s32 mode;           /* 0x14 */
    s32 unk18;          /* 0x18 */
    u8 pad1C[4];
    Stats *atk;         /* 0x20 */
    f32 unk24;          /* 0x24 */
    s32 unk28;          /* 0x28 */
} Hit;

typedef struct Info {
    s32 dmg;            /* 0x00 */
    s32 type;           /* 0x04 */
    s32 unk8;           /* 0x08 */
    s32 unkC;           /* 0x0C */
    f32 unk10;          /* 0x10 */
    s32 flags;          /* 0x14 */
    s32 flags2;         /* 0x18 */
    s32 status;         /* 0x1C */
    f32 unk20;          /* 0x20 */
    s8 unk24;           /* 0x24 */
    s8 unk25;           /* 0x25 */
} Info;



#define RAND() ((f32)(rand() >> 15) * (1.0f / 65536.0f))

void BtlHitCalcDamage(Hit *hit, Info *info) {
    Skill *sk;
    Unit *unit;
    Stats *def;
    Stats *atk;
    Stats *st;
    Stats *st0;
    Stats *atk2;
    f32 dmg;
    f32 mult;
    f32 a;
    f32 d;
    f32 r;
    f32 rate;
    f32 c;
    f32 c2;
    f32 t;
    s32 i;
    s32 k;
    s32 n;
    u32 bit;
    s32 max;

    sk = hit->skill;
    dmg = 0.0f;
    info->type = (sk->unk1 & 1) ? 0x42 : 0x41;
    info->unkC = 0;
    info->unk8 = 0;
    info->unk10 = dmg;
    info->flags = hit->flags;
    info->unk24 = sk->kind;
    if (hit->target < 4 && hit->mode < 2) {
        info->unk25 = hit->atk->unk16D;
    } else {
        info->unk25 = sk->unk16;
    }

    if (sk->kind < 6) {
        unit = hit->unit;
        atk = hit->atk;
        def = unit->stats;
        a = 0.0f;
        mult = 1.5f;
        if (sk->kind >= 4) {
            r = def->unk250[info->unk25];
            if (r < a) {
                info->flags |= 0x100;
                mult = -r * mult;
            } else {
                mult = r * mult;
                if (r > 1.01f) {
                    info->flags |= 0x10;
                } else if (r < 0.99f) {
                    info->flags |= 0x20;
                }
            }
            info->unk8 = sk->unkA;
            info->unkC = sk->unkC;
            if (hit->target < 4) {
                if (hit->mode == 1) {
                    info->unk8 = sk->unkA + atk->unk17C;
                    info->unkC = sk->unkC + atk->unk17C;
                } else if (hit->mode == 0) {
                    info->unk8 = sk->unkA + atk->unk24C;
                }
            }
            if (hit->flags & 0x20000) {
                info->unkC += atk->unk1A4;
            }
            info->unk8 = info->unk8 * (def->unk250[info->unk25] * def->unk274);
            info->unk8 = info->unk8 * def->unk2A8;
            info->unk10 = sk->unkE * def->unk2AC;
        }
        if (hit->mode == 2 && hit->target < 4) {
            if (sk->kind < 4) {
                a = sk->unk2;
                info->flags |= 0x100;
                d = 0.0f;
                mult = 1.0f;
                if (sk->kind == 2) {
                    info->flags |= 0x40;
                } else if (sk->kind == 3) {
                    info->flags |= 0x80;
                }
            } else {
                if (sk->kind == 5) {
                    a = sk->unk2 * 1.25f;
                    d = def->unk5C;
                    if (unit->unk1B2.w & 0x400) {
                        d *= 1.5f;
                    } else if (unit->unk1B2.w & 4) {
                        d *= 0.5f;
                    }
                } else {
                    a = sk->unk2 * 1.25f;
                    d = def->unk54;
                    if (unit->unk1B2.w & 0x200) {
                        d *= 1.5f;
                    } else if (unit->unk1B2.w & 2) {
                        d *= 0.5f;
                    }
                }
                if (sk->unk15 != 0 && !(info->flags & 0x100)) {
                    for (i = 0; i < 2; i++) {
                        if (def->unk8[i] == sk->unk15) {
                            info->flags |= 0x10;
                            d *= 0.0f;
                            break;
                        }
                    }
                }
            }
            switch (ptyItemTable[hit->unk18].type) {
            case 1:
                mult *= atk->unk160[0];
                break;
            case 2:
                mult *= atk->unk160[1];
                break;
            case 3:
                mult *= atk->unk160[2];
                break;
            }
        } else {
            if (sk->kind < 4) {
                a = atk->unk58 + sk->unk2;
                if (hit->target < 4 && (hit->mode == 4 || hit->mode == 6)) {
                    a += atk->unk190;
                }
                if (info->unk25 != 0 && !((hit->mode == 4 || hit->mode == 6) && hit->target < 4)) {
                    a += atk->unk13C[info->unk25];
                    mult *= atk->unk118[info->unk25];
                }
                info->flags |= 0x100;
                d = 0.0f;
                a *= 1.25f;
                if (sk->kind == 2) {
                    info->flags |= 0x40;
                } else if (sk->kind == 3) {
                    info->flags |= 0x80;
                }
            } else if (sk->kind == 5) {
                a = atk->unk58;
                if (hit->target < 4 && (hit->mode == 4 || hit->mode == 6)) {
                    a += atk->unk190;
                }
                if (info->unk25 != 0 && !((hit->mode == 4 || hit->mode == 6) && hit->target < 4)) {
                    a += atk->unk13C[info->unk25];
                    mult *= atk->unk118[info->unk25];
                }
                d = def->unk5C;
                a *= sk->unk2 * 0.01f * 1.25f;
                if (unit->unk1B2.w & 0x400) {
                    d *= 1.5f;
                } else if (unit->unk1B2.w & 4) {
                    d *= 0.5f;
                }
            } else {
                a = atk->unk50;
                if (hit->target < 4 && (hit->mode == 4 || hit->mode == 6)) {
                    a += atk->unk190;
                }
                a *= sk->unk2 * 0.01f * 1.25f;
                if (btlUnits[hit->target].powUp & 0x100) {
                    a *= 1.4f;
                } else if (btlUnits[hit->target].powUp & 1) {
                    a *= 0.6f;
                }
                d = def->unk54;
                max = 0;
                for (i = 0; i < 2; i++) {
                    if (max < def->unkE4[atk->unk8[i]]) {
                        max = def->unkE4[atk->unk8[i]];
                    }
                }
                d += max;
                if (max > 0) {
                    info->flags |= 0x20;
                }
                if (unit->unk1B2.w & 0x200) {
                    d *= 1.5f;
                } else if (unit->unk1B2.w & 2) {
                    d *= 0.5f;
                }
                if (atk->unk16E != 0 && hit->mode < 2 && !(info->flags & 0x100)) {
                    for (i = 0; i < 2; i++) {
                        if (def->unk8[i] == atk->unk16E) {
                            info->flags |= 0x10;
                            d *= 0.0f;
                            break;
                        }
                    }
                }
                c = 1.0f;
                r = 0.0f;
                for (i = 0; i < 2; i++) {
                    if (r < atk->unk7C[def->unk8[i]]) {
                        r = atk->unk7C[def->unk8[i]];
                    }
                }
                c += r;
                r = 0.0f;
                for (i = 0; i < 2; i++) {
                    if (def->unkB0[atk->unk8[i]] < r) {
                        r = def->unkB0[atk->unk8[i]];
                    }
                }
                c += r;
                if (c > 1.01f) {
                    info->flags |= 0x10;
                } else if (c < 0.99f) {
                    if (c < 0.0f) {
                        c = 0.0f;
                    }
                    info->flags |= 0x20;
                }
                mult *= c;
            }
            mult *= sk->unk4 * 0.01f;
        }
        dmg = (a - d) * mult;
        if (dmg < 0.0f) {
            dmg = 0.0f;
        }
    }

    atk2 = hit->atk;
    st0 = hit->unit->stats;
    if (hit->unk24 > 0.0f) {
        info->flags2 = hit->unk28;
    } else {
        info->flags2 = sk->unk10;
    }
    info->status = sk->unk17;
    if (st0->flags & 0x4000) {
        if (info->flags2 != 0) {
            info->flags2 = 0;
            if (!(info->flags & 0x200)) {
                info->flags |= 0x400;
            }
        }
        info->status = 0;
        if (sk->kind != 4 && sk->kind != 5 && sk->kind != 7) {
            info->flags |= 0x200;
        }
    } else {
        if (sk->kind == 4 || sk->kind == 5 || sk->kind == 7) {
            info->unk20 = -st0->unk5C;
        } else {
            info->status <<= 8;
            info->unk20 = 0.0f;
            info->flags |= 0x200;
        }
        if (hit->mode < 2 && hit->target < 4) {
            info->unk20 = 1500.0f;
        } else if (hit->mode == 2 && hit->target < 4) {
            info->unk20 += sk->unk6;
        } else {
            info->unk20 += atk2->unk58 + sk->unk6;
            if (hit->target < 4 && (hit->mode == 4 || hit->mode == 6)) {
                info->unk20 += atk2->unk190;
            }
            if (sk->kind == 5 && info->unk25 != 0 && !((hit->mode == 4 || hit->mode == 6) && hit->target < 4)) {
                info->unk20 += atk2->unk13C[info->unk25];
                info->unk20 *= atk2->unk118[info->unk25];
            }
            info->unk20 *= sk->unk8 * 0.01f;
        }
        if (info->unk20 < 0.0f) {
            info->unk20 = 0.0f;
        }
    }

    if (hit->unk24 > 0.0f) {
        rate = hit->unk24;
    } else {
        rate = sk->unk14 * 0.01f;
    }

    if (!(info->flags & 0x200)) {
        if ((hit->unit->unk78 & 4) || ((st = hit->unit->stats)->unk1CC > 0.0f && RAND() <= st->unk1CC)) {
            dmg *= hit->unit->stats->unk6C;
            info->type = 0x43;
            info->flags |= 4;
            info->unk8 = 0;
            info->unkC = 0;
            info->unk10 *= hit->unit->stats->unk6C;
            rate *= hit->unit->stats->unk6C;
        } else if (hit->unit->unk78 & 1) {
            if (!(info->flags & 0x100)) {
                if ((info->flags & 2) || (st = hit->unit->stats)->unk1F4 <= 0.0f || st->unk1F4 < RAND()) {
                    info->flags |= 2;
                    dmg *= 1.3f;
                }
            }
        }
        if (hit->target < 4 && hit->unit->id < 4) {
            dmg *= 0.33333334f;
        }
        n = BtlHudGaugeGetHits(hit->unit->id);
        if (n > 0) {
            t = n * 0.03f * dmg + dmg;
            dmg = t;
        }
    }

    if (rate <= 0.0f) {
        info->flags2 = 0;
    }
    if (info->flags2 != 0) {
        for (bit = 1; bit < 0x10; bit <<= 1) {
            if (info->flags2 & bit) {
                if (rate < RAND()) {
                    info->flags2 &= ~bit;
                }
            }
        }
        for (k = 0; k < 12; k++, bit <<= 1) {
            if (info->flags2 & bit) {
                r = RAND();
                c2 = rate;
                if (!(info->flags & 0x200)) {
                    c2 = rate * hit->unit->stats->unk278[k];
                }
                if (c2 <= 0.0f || c2 < r) {
                    info->flags2 &= ~bit;
                    if (r <= rate) {
                        info->flags |= 0x400;
                    }
                }
            }
        }
    }

    if (!(btlDebugCfg.flags & 1)) {
        if (btlActingUnit == 0xFF) {
            dmg *= RAND() * 0.2f + 0.9f;
        } else {
            dmg *= RAND() * 0.1f + 0.95f;
        }
    }
    info->dmg = dmg;
    if (info->dmg <= 0) {
        if (RAND() < 0.3f) {
            info->dmg = 1;
        } else {
            info->dmg = 0;
        }
    }
}
