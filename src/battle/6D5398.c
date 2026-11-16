/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n33.h"
#include "party.h"

void BtlHitCalcDamage(BtlHit *, BtlHitResult *);

#define RAND() ((f32)(rand() >> 15) * (1.0f / 65536.0f))
#define ROUND(x) ((x) == (s32)(x) ? (x) : (0.0f < (x) ? (s32)(x) + 1 : -(s32)(-x) - 1))
#define CLAMP(v, hi) ((v) < 0 ? 0 : ((hi) < (v) ? (hi) : (v)))

s32 BtlHitResolve(BtlHit *hit) {
    BtlHitResult info;
    VuVec4 tmp;
    BtlUnit *unit;
    PtyUnit *st;
    BtlUnit *other;
    BtlUnit *tgt;
    PtyUnit *atk;
    PtyMoveEffect *skill;
    f32 chance;
    f32 rate;
    f32 x;
    s32 n;
    s32 i;
    s32 a3;
    s32 ok;
    s32 v;
    s32 fl;

    unit = hit->target;
    st = unit->params;
    if (hit->effect != NULL) {
        BtlHitCalcDamage(hit, &info);
        if (st->status & 0x8000) {
            if (!(info.effects & 0x8000) || !(info.flags & 0x200)) {
                return 0;
            }
        } else if (!(info.flags & 0x200)) {
            if (btlPhase != 0x80) {
                return 0;
            }
            if (btlActingUnit != 0xFC) {
                if (hit->src.unit == unit->id) {
                    return 0;
                }
                if (unit->body >= 0) {
                    if (hit->src.unit == unit->body || hit->src.body == unit->body) {
                        return 0;
                    }
                } else if (hit->src.body == unit->id) {
                    return 0;
                }
                if (unit->id == btlActingUnit) {
                    return 0;
                }
            }
            unit->attacker = hit->src.unit;
            if (btlActingUnit == 0xFF) {
                if (unit->body >= 0) {
                    other = &btlUnits[unit->body];
                } else {
                    other = unit;
                }
                if (other->state == 0x44) {
                    return 0;
                }
                if (other->state == 0x80 && unit->state == other->state && !(st->status & 4)) {
                    if (unit->flags & 0x10) {
                        chance = st->avoidEvade;
                    } else {
                        chance = st->avoidWait;
                    }
                    if (0.0f < chance) {
                        if (unit->powUp & 0x1000) {
                            chance += 0.3f;
                        } else if (unit->powUp & 0x10) {
                            chance -= 0.6f;
                        }
                        if (RAND() < chance) {
                            other->retState = other->state;
                            other->retPhase = other->phase;
                            if (unit->body >= 0) {
                                other->attacker = unit->attacker;
                                unit->retState = unit->state;
                                unit->retPhase = unit->phase;
                                BtlUnitSetState(unit, 0x7E);
                            }
                            BtlUnitSetState(other, 0x44);
                            n = unit->mvp;
                            unit->mvp = (unit->flags & 0x10) ? n + 2 : n + 1;
                            return 0;
                        }
                    }
                }
            }
        }
    } else {
        a3 = hit->src.status;
        if (st->status & 0x8000) {
            if (!(a3 & 0x8000)) {
                return 0;
            }
            if (!(hit->flags & 0x200)) {
                return 0;
            }
        }
        info.reaction = (hit->flags & 0x40000) ? 0x42 : 0x41;
        info.flags = hit->flags;
        info.knockback = hit->unk5C;
        info.damage = hit->damage;
        info.power = hit->unk60;
        info.unkC = 0;
        info.ipDelta = 0;
        info.effects = a3;
        info.status = hit->status;
        info.unk24 = (a3 == 0) ? ((hit->status == 0) ? 4 : 6) : 6;
        info.hitAttr = hit->unk58;
    }

    if (info.flags & 0x400) {
        BtlHudMarkerSpawn(unit, 4);
    }
    if ((info.effects & 0x8000) && (info.flags & 0x200) && !(st->status & 0x8000)) {
        info.status = 0;
        info.effects = 0x8000;
    }
    if (info.effects != 0) {
        BtlHitApplyAilment(unit, &info);
    }

    if (!(st->status & 0x8000)) {
        skill = hit->effect;
        if (skill != NULL && skill->stat != 0) {
            if (skill->stat >= 0x1F && skill->stat <= 0x28) {
                
                if (BtlHitDrainHp(&hit->src, unit, ROUND(info.damage * ((skill->stat - 0x1E) * 0.1f))) == 0) {
                    return 1;
                }
            } else {
                switch (skill->stat) {
                    case 14:
                        st->status = BtlUnitMaskImmuneStatus(unit, st->status2);
                        info.status = 0;
                        unit->powUp = 0;
                        unit->vanish = 0;
                        unit->bind = 0;
                        unit->sealM = 0;
                        unit->sealS = 0;
                        unit->confu = 0;
                        unit->sleep = 0;
                        unit->regene = 0;
                        unit->antiMagic = 0;
                        unit->transp = 0;
                        for (i = 4; i >= 0; i--) {
                            unit->statMod[i] = 0;
                        }
                        unit->wall = 0;
                        break;
                    case 15:
                        if (st->antiRetire <= 0.0f || st->antiRetire < RAND()) {
                            if (!(unit->flags & 4) || RAND() < st->guardDec) {
                                unit->flags |= 0x40;
                                BtlUnitApplyDamage(unit, st->maxHp, 0, 0);
                                if (st->hp <= 0) {
                                    unit->powUp = 0;
                                    info.effects = 0x8000;
                                    st->status = 0x8000;
                                    info.status = 0;
                                    if (unit->id < 4) {
                                        IpPanelSetMode(unit->id, 0x12, 1);
                                    }
                                } else {
                                    unit->flags &= 0xFFFFFFBF;
                                }
                            }
                        } else {
                            BtlHudMarkerSpawn(unit, 4);
                        }
                        break;
                    case 16:
                        if (unit->id < 4) {
                            if ((st->antiLvDrain <= 0.0f || st->antiLvDrain < RAND()) &&
                                (!(unit->flags & 4) || RAND() < st->guardDec)) {
                                PtyCharLevelDown(st);
                            } else {
                                BtlHudMarkerSpawn(unit, 4);
                                break;
                            }
                        }
                        BtlHudMarkerSpawn(unit, 5);
                        break;
                    case 18:
                        if (!(unit->flags & 0x80)) {
                            if (hit->src.unit == unit->id) {
                                unit->ipRet -= info.power / 10000.0f;
                                if (unit->ipRet < 0.0f) {
                                    unit->ipRet = 0.0f;
                                }
                            } else {
                                unit->ipPos -= info.power / 10000.0f;
                                if (unit->ipPos < 0.0f) {
                                    unit->ipPos = 0.0f;
                                }
                            }
                            BtlHitAddMvpPoints(hit, 10);
                        }
                        break;
                    case 21:
                        if (!(st->status & 0x4000)) {
                            x = RAND();
                            if (x < 0.2f) {
                                info.status |= 1;
                            } else if (x < 0.4f) {
                                info.status |= 2;
                            } else if (x < 0.6f) {
                                info.status |= 4;
                            } else if (x < 0.8f) {
                                info.status |= 8;
                            } else {
                                info.status |= 0x10;
                            }
                            if (info.power < 2500.0f) {
                                info.power = 2500.0f;
                            }
                        }
                        break;
                    case 23:
                        info.flags |= 0x40;
                        break;
                    case 24:
                        info.flags |= 0x80;
                        break;
                    case 41:
                        if (st->antiHalf < 1.0f && st->hp >= 2) {
                            chance = skill->chance;
                            chance *= 0.01f;
                            chance *= 1.0f - st->antiHalf;
                            if (RAND() < chance) {
                                info.damage = st->hp / 2;
                            } else {
                                info.damage = 0;
                            }
                        } else {
                            info.damage = 0;
                        }
                        info.flags &= 0xFFFFFFCF;
                        break;
                }
            }
        }

        if (info.status != 0) {
            BtlHitApplyStatChange(unit, &info);
            BtlHudStatChangeShow(unit, info.status);
            if (info.status & 0x1F00) {
                SndSePlay3D(0x7EA, 0, 8, unit->pos);
            } else {
                SndSePlay3D(0x7EB, 0, 8, unit->pos);
            }
            BtlHitAddMvpPoints(hit, 2);
        }
        if (info.effects != 0) {
            if (info.flags & 0x200) {
                BtlHitAddMvpPoints(hit, (info.effects & 0x8000) ? 5 : 2);
                if (info.effects & 6) {
                    BtlHitAddMvpPoints(hit, 3);
                }
            } else {
                if (info.effects & 0x70) {
                    BtlHitAddMvpPoints(hit, 1);
                }
                if (info.effects & 0x700) {
                    BtlHitAddMvpPoints(hit, 2);
                }
                if (info.effects & 0x1080) {
                    BtlHitAddMvpPoints(hit, 4);
                }
                if (info.effects & 0x2000) {
                    BtlHitAddMvpPoints(hit, 10);
                }
                if (info.effects & 0x4000) {
                    BtlHitAddMvpPoints(hit, 20);
                }
            }
        }

        if (info.flags & 0x100) {
            if (info.flags & 0x40) {
                st->mp += info.damage;
                st->mp = CLAMP(st->mp, st->maxMp);
                BtlUnitAddManaEggMp(unit, info.damage);
            } else if (info.flags & 0x80) {
                st->sp += info.damage;
                st->sp = CLAMP(st->sp, st->maxSp);
            } else {
                BtlHudGaugeAddHit(-1, unit, 0);
                st->hp += info.damage;
                st->hp = CLAMP(st->hp, st->maxHp);
                BtlHitAddMvpPoints(hit, 2);
            }
            BtlChrGetPartPos(&tmp, unit, 1);
            if (!(info.flags & 0x2000)) {
                if (info.flags & 0x40) {
                    if (info.damage < 100) {
                        v = (info.damage >= 20) ? 0x12 : 0x13;
                    } else {
                        v = 0x11;
                    }
                } else if (info.flags & 0x80) {
                    if (info.damage < 100) {
                        v = (info.damage >= 20) ? 0x16 : 0x17;
                    } else {
                        v = 0x15;
                    }
                } else if (info.damage < 1500) {
                    v = (info.damage >= 500) ? 0xE : 0xF;
                } else {
                    v = 0xD;
                }
                PtclBurstSpawnAtUnit(v, unit, unit->rot[1]);
                SndSePlay3D(0x7D8, 0, 8, unit->pos);
            }
            if (info.flags & 0x100000) {
                if (btlHitDamageAccum[unit->id] < 0) {
                    btlHitDamageAccum[unit->id] = info.damage;
                } else {
                    btlHitDamageAccum[unit->id] += info.damage;
                }
            } else if (!(info.flags & 0x1000)) {
                if (btlHitDamageAccum[unit->id] < 0) {
                    ((void (*)(s32, VuVec4 *))BtlHudDamageNumSpawn)(info.damage, &tmp); /* casts: a2 left as it is (see the definition) */
                } else {
                    ((void (*)(s32, VuVec4 *))BtlHudDamageNumSpawn)(info.damage + btlHitDamageAccum[unit->id], &tmp);
                    btlHitDamageAccum[unit->id] = -1;
                }
            }
            if (hit->effect != NULL && !(info.effects & 1)) {
                unit->transp = 0;
                st->status &= 0xFFFFFFFE;
            }
            if (info.ipDelta < 0 && !(unit->flags & 0x80)) {
                unit->ipPos += info.ipDelta / 10000.0f;
                if (unit->ipPos < 0.0f) {
                    unit->ipPos = 0.0f;
                }
            }
            if (hit->victimHandler != 0 || unit->state == 0x7F) {
                info.flags |= 0x1000;
                BtlHitShowEffect(unit, hit, &info, info.hitAttr);
                if (!(info.flags & 0x8000)) {
                    ((void (*)(BtlUnit *, s32, f32, s32))BtlUnitHitReact)(unit, hit->angle, info.knockback, info.reaction); /* float-first cast: EABI argument-load order */
                    if (hit->victimHandler != 0) {
                        if (unit->body >= 0) {
                            (btlUnits + unit->body)->stateHandler = hit->victimHandler;
                            (btlUnits + unit->body)->state = 0x7F;
                        } else {
                            unit->stateHandler = hit->victimHandler;
                            unit->state = 0x7F;
                        }
                    }
                }
            }
        }
        if ((info.flags & 0x100) || info.unk24 >= 6) {
            if (info.flags & 0x200) {
                return 1;
            }
            if (!(info.effects & 0x21E0)) {
                return 1;
            }
            IpIconBounce(unit->id, 8.0f);
            BtlUnitHitReact(unit, unit->rot[1], info.reaction, 0.0f);
            if (info.unkC > 0 && unit->ipMode == 2) {
                BtlUnitApplyDamage(unit, 0, 0, info.unkC);
            }
            return 1;
        }
        if (hit->flags & 0x10000) {
            rate = hit->src.params->absorbHp;
            if (0.0f < rate) {
                if (BtlHitDrainHp(&hit->src, unit, ROUND(info.damage * rate)) == 0) {
                    return 1;
                }
            }
        }
        if (info.unkC > 0 && unit->ipMode == 2) {
            if (0.0f < st->antiCancel && RAND() <= st->antiCancel) {
                info.unkC = 0;
                info.flags |= 8;
            } else {
                info.flags |= 1;
                BtlHitAddMvpPoints(hit, 0x19);
            }
        } else {
            info.unkC = 0;
        }
    } else {
        BtlHitAddMvpPoints(hit, 5);
        if (info.unk24 == 7) {
            return 1;
        }
    }

    if (hit->effect != NULL) {
        if (!(info.flags & 0xC0)) {
            ok = 1;
            if (st->status & 4) {
                unit->wall -= info.damage;
                if (unit->wall <= 0) {
                    st->status &= 0xFFFFFFFB;
                    ok = 0;
                    unit->wall = 0;
                    info.reaction = 0x42;
                }
                FxWallHit(unit);
                info.damage = 0;
            } else if (info.damage <= st->noDamage) {
                info.damage = 0;
            }
            if (info.damage <= 0 && ok && info.reaction != 0x43) {
                info.reaction = 0x45;
            }
        }
        if (0.0f < st->sArmorRate && RAND() <= st->sArmorRate) {
            if (info.reaction != 0x43) {
                if (info.unkC > 0 || hit->victimHandler != 0 || unit->state == 0x7F || (unit->params->status & 0x20E0)) {
                    info.reaction = 0x45;
                } else {
                    info.flags |= 0x8000;
                }
            }
            info.ipDelta = 0;
        }
        if (!(info.flags & 0x8000)) {
            if ((info.flags & 0x200) || !(info.effects & 0x20)) {
                if ((rand() >> 15) < 0x8000) {
                    fl = st->status;
                    st->status = fl & 0xFFFFFFDF;
                    if (!(fl & 0xC0)) {
                        unit->sleep = 0;
                    }
                }
            }
            if ((info.flags & 0x200) || !(info.effects & 0x100)) {
                if (!(st->status2 & 0x100) && (rand() >> 15) < 0x8000) {
                    unit->confu = 0;
                    st->status &= 0xFFFFFEFF;
                }
            }
            if (!(info.effects & 1)) {
                unit->transp = 0;
                st->status &= 0xFFFFFFFE;
            }
        }
        if (!(st->status & 0x2080)) {
            switch (info.reaction) {
                case 0x42:
                    st->sp += st->spSupDown;
                    st->sp = CLAMP(st->sp, st->maxSp);
                    if (0.0f < st->angryRate) {
                        st->angry += (2.0f * RAND() + 6.0f) * st->angryRate;
                    }
                    break;
                case 0x43:
                    st->sp += st->spSupGuard;
                    st->sp = CLAMP(st->sp, st->maxSp);
                    if (0.0f < st->angryRate) {
                        st->angry += (2.0f * RAND() + 5.0f) * st->angryRate;
                    }
                    break;
                default:
                    st->sp += st->spSupDmg;
                    st->sp = CLAMP(st->sp, st->maxSp);
                    if (0.0f < st->angryRate) {
                        st->angry += (2.0f * RAND() + 4.0f) * st->angryRate;
                    }
                    break;
            }
        }
        if (btlActingUnit == 0xFF && hit->effect->kind == 4 && !(hit->flags & 0x400000) &&
            ((0.0f < st->refDamage && RAND() <= st->refDamage) ||
             (atk = hit->src.params, 0.0f < atk->atkRef && RAND() <= atk->atkRef))) {
            tgt = &btlUnits[hit->src.unit];
            BtlHitQueueAddDamage(tgt, (f32)info.damage * 0.1f, 0.0f, tgt->rot[1], 0x46800, 0);
            info.flags |= 0x80000;
        }
        btlHitUnitMask |= 1 << unit->id;
    }
    BtlHitShowEffect(unit, hit, &info, info.hitAttr);
    if (!(info.flags & 0x8000)) {
        ((void (*)(BtlUnit *, s32, f32, s32))BtlUnitHitReact)(unit, hit->angle, info.knockback, info.reaction); /* float-first cast: EABI argument-load order */
        if (hit->victimHandler != 0) {
            if (unit->body >= 0) {
                (btlUnits + unit->body)->stateHandler = hit->victimHandler;
                (btlUnits + unit->body)->state = 0x7F;
            } else {
                unit->stateHandler = hit->victimHandler;
                unit->state = 0x7F;
            }
        }
    }
    IpIconBounce(unit->id, 8.0f);
    if (info.flags & 0x40) {
        st->mp -= info.damage;
        st->mp = CLAMP(st->mp, st->maxMp);
        BtlUnitAddManaEggMp(unit, -info.damage);
        info.damage = 0;
    } else if (info.flags & 0x80) {
        st->sp -= info.damage;
        st->sp = CLAMP(st->sp, st->maxSp);
        info.damage = 0;
    }
    if (info.damage > 0) {
        BtlHitAddMvpPoints(hit, 1);
    }
    if (info.flags & 2) {
        BtlHitAddMvpPoints(hit, 10);
    } else if (info.flags & 4) {
        unit->mvp += 2;
    }
    BtlUnitApplyDamage(unit, info.damage, info.ipDelta, info.unkC);
    return 1;
}
