/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"

void VuVecCopy(void *, void *);

void BtlUnitApplyDamage(BtlUnit *u, s32 dmg, s32 a2, s32 a3) {
    PtyUnit *st = u->params;
    BtlUnit *o;
    BtlUnit *p;
    s32 i;
    s32 j;
    s32 k;
    s32 t;
    s16 m;
    f32 f;

    if (dmg > 0 && u->id < 4) {
        IpPanelFlash(u->id);
    }
    if (st->status & 0x8000) {
        return;
    }
    if (dmg >= st->hp) {
        if (btlDebugCfg.flags & 2) {
            st->hp = st->maxHp;
            st->mp = st->maxMp;
            st->sp = st->maxSp;
            st->status = BtlUnitMaskImmuneStatus(u, st->status2);
            u->powUp = 0;
            u->transp = u->antiMagic = u->regene = u->sleep = u->confu = u->sealS = u->sealM = u->bind = u->vanish = 0;
            for (j = 4; j >= 0; j--) {
                u->statMod[j] = 0;
            }
            u->wall = 0;
            FxWallHit(u);
        } else {
            st->hp = 0;
            if ((btlEncounterFlags & 0x10) && u->id >= 4 &&
                ((t = u->params->id & 0xF) == 1 || t == 5)) {
                o = &btlUnits[4];
                for (i = 11; i >= 0; i--, o++) {
                    if (o->params->hp > 0) {
                        o->attacker = u->attacker;
                        BtlUnitApplyDamage(o, o->params->maxHp, 0, 0);
                    }
                }
            } else if (u->parts[0] >= 0 && (u8)u->params->id != 0x23) {
                for (k = 0; k < 3; k++) {
                    if (u->parts[k] >= 0) {
                        p = &btlUnits[u->parts[k]];
                        if (p->state >= 0x40) {
                            p->attacker = u->attacker;
                            BtlUnitApplyDamage(p, p->params->maxHp, 0, 0);
                        }
                    }
                }
            } else if (u->body >= 0) {
                o = &btlUnits[u->body];
                if (o->partsChr == u->id) {
                    o->retState = o->rtAct;
                    o->retPhase = o->rtP;
                    VuVecCopy(o->moveTarget, o->pos);
                    o->partsChr = -1;
                }
            }
            return;
        }
    } else {
        st->hp -= dmg;
    }
    if (dmg > 0) {
        u->flag |= 4;
        if (!(u->flag & 0x20000000) && (f32)st->hp <= (f32)st->maxHp * 0.3f) {
            if (u->flag >= 0) {
                u->flag |= 8;
            }
            if (!(u->flag & 0x40000000)) {
                u->flag |= 0x10;
            }
            u->flag |= 0xE0000020;
        } else if (!(u->flag & 0x40000000) && (f32)st->hp <= (f32)st->maxHp * 0.5f) {
            if (u->flag >= 0) {
                u->flag |= 8;
            }
            u->flag |= 0xC0000010;
        } else if (u->flag >= 0 && (f32)st->hp <= (f32)st->maxHp * 0.7f) {
            u->flag |= 0x80000008;
        }
    }
    if (a3 > 0) {
        SndSePlay3D(0x7E0, 0, 8, u->pos);
        u->ipRet = (f32)a3 * 0.0001f;
        u->ipPos = u->ipRet;
        u->flags &= 0xFFFFFF7F;
        u->ipMode = 1;
        u->ipSpeed = 0.0f;
        u->scopeType = 0;
        u->ipSpeed = (f32)u->params->params[4] * btlInvPartyAvgSpeed / ((f32)btlPartyAliveCount * 60.0f + 240.0f);
        u->ipMode = 0;
        u->action = 0x7F;
        BtlSchedActCancel(u->id);
        if (!(u->params->status & 0x100) && u->id >= 4) {
            u->flag |= 1;
            AiEnemyChooseCommand(u);
        }
        if (u->parts[0] >= 0) {
            if (u->partsChr != u->id) {
                u->rtAct = 0x80;
                u->rtP = 1;
            } else {
                u->partsChr = -1;
            }
        } else if (u->body >= 0) {
            o = &btlUnits[u->body];
            if (o->partsChr == u->id) {
                BtlSchedActCancel(o->id);
                o->retState = o->rtAct;
                o->retPhase = o->rtP;
                VuVecCopy(o->moveTarget, o->pos);
                o->partsChr = -1;
            }
        } else {
            u->partsChr = -1;
        }
        if (u->partsChr < 0) {
            u->retState = 0x80;
            u->retPhase = 1;
        }
        if (u->comboPartners[0] >= 0) {
            BtlUnitCancelAction(u);
        } else if (u->comboLeader >= 0) {
            BtlUnitCancelAction(&btlUnits[u->comboLeader]);
        }
        GphScreenFadeSetColor(255.0f, 255.0f, 255.0f, 96.0f);
        GphScreenFadeStart(0, 255.0f, 255.0f, 255.0f, 0.0f, 16.0f);
        if (btlActingUnit == 0xFF) {
            btlSlowMotionRate = 2;
            btlSlowMotionTimer = 0x30;
        }
    } else if (a2 > 0) {
        if (u->ipPos > 0.0f) {
            SndSePlay3D(0x7DF, 0, 8, u->pos);
            if (u->ipMode < 2) {
                f = u->ipRate;
                u->ipDamage += (f32)a2 * 0.0001f * f;
            } else {
                f = u->ipRate;
                u->ipDamage += (f32)a2 * 0.0002f * f;
            }
            u->ipRate = f - 0.25f;
            if (u->ipRate < 0.25f) {
                u->ipRate = 0.25f;
            }
        }
    }
    if (a2 > 0 || a3 > 0) {
        m = (u->state == 0x42) ? st->ipStopDown : st->ipStopDmg;
        if (u->ipStop < m) {
            u->ipStop = m;
        }
    }
    if (btlPartyHurtLevel == 0 && dmg > 0 && u->id < 4) {
        btlPartyHurtLevel = 1;
    }
}
