#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"


#define RAND() ((f32)(rand() >> 15) * (1.0f / 65536.0f))

void BtlHitApplyAilment(BtlUnit *u, BtlHitResult *info) {
    PtyUnit *st = u->params;
    s32 i;
    s32 j;
    BtlUnit *p;

    if (!(st->status & 0x8000)) {
        if (info->effects & 1) {
            st->status |= 1;
            u->transp = u->transp + info->power * (RAND() * 0.2f + 0.9f) * st->kregGood;
        }
        if (info->effects & 2) {
            st->status |= 2;
            u->antiMagic = u->antiMagic + info->power * (RAND() * 0.2f + 0.9f) * st->kregGood;
        }
        if (info->effects & 4) {
            FxWallStart(u);
            st->status |= 4;
            u->wall = u->wall + info->power * (RAND() * 0.2f + 0.9f);
        }
        if (info->effects & 8) {
            st->status |= 8;
            u->regene = u->regene + info->power * (RAND() * 0.2f + 0.9f) * st->kregGood;
            u->exeGood = st->exeGood * (RAND() * 0.8f + 0.2f);
        }
        if (!(info->flags & 0x200)) {
            if (info->effects & 0x8000) {
                BtlUnitApplyDamage(u, st->maxHp, 0, 0);
                if (st->hp <= 0) {
                    u->powUp = 0;
                    info->effects = 0x8000;
                    st->status = 0x8000;
                    info->status = 0;
                }
                SndSePlay3D(0x7E4, 0, 8, u->pos);
            } else if (info->effects & 0x4000) {
                u->powUp = 0;
                info->effects = 0x4000;
                st->status = 0x4000;
                info->status = 0;
                u->vanish = u->vanish + info->power * (RAND() * 0.2f + 0.9f) * st->kregOther;
                u->transp = u->antiMagic = u->regene = u->sleep = u->confu = u->sealS = u->sealM = u->bind = 0;
                for (i = 4; i >= 0; i--) {
                    u->statMod[i] = 0;
                }
                u->wall = 0;
                FxWallHit(u);
                if (u->id < 4) {
                    PtyUnitCalcParams(u->params, 0, 0);
                } else {
                    PtyUnitCalcParams(u->params, u->def->data, &ptyBattleParams[u->id - 4]);
                }
            } else {
                if (info->effects & 0x80) {
                    if (!(st->status & 0x80)) {
                        u->sleep = 0;
                    }
                } else if (info->effects & 0x40) {
                    if (!(st->status & 0xC0)) {
                        u->sleep = 0;
                    }
                }
                st->status |= info->effects;
                if (u->id >= 4) {
                    p = btlUnits;
                    for (i = 0; i < 16; i++, p++) {
                        if (p->state > 0) {
                            p->flag |= 0x80;
                        }
                    }
                }
                if (st->status & 0x800) {
                    st->status &= 0xFFFFFFEF;
                    info->effects &= 0xFFFFFFEF;
                }
                if (st->status & 0x2000) {
                    u->sleep = 0;
                    st->status &= 0xFFFFFF1F;
                    info->effects &= 0xFFFFFF1F;
                } else if (st->status & 0x80) {
                    st->status &= 0xFFFFFF9F;
                    info->effects &= 0xFFFFFF9F;
                } else if (st->status & 0x40) {
                    st->status &= 0xFFFFFFDF;
                    info->effects &= 0xFFFFFFDF;
                }
                if (info->effects != 0) {
                    SndSePlay3D(0x7DE, 0, 8, u->pos);
                }
                if (info->effects & 0x810) {
                    u->exeBad = st->exeBad * (RAND() * 0.8f + 0.2f);
                }
                if (info->effects & 0x60) {
                    u->sleep = u->sleep + info->power * (RAND() * 0.2f + 0.9f) * st->kregSpi;
                    info->unkC = 5000;
                    u->flags &= 0xFFFFFFFB;
                }
                if (info->effects & 0x80) {
                    u->sleep = u->sleep + info->power * (RAND() * 0.2f + 0.9f) * st->kregOther;
                    u->flags &= 0xFFFFFFFB;
                }
                if (info->effects & 0x100) {
                    u->confu = u->confu + info->power * (RAND() * 0.2f + 0.9f) * st->kregSpi;
                    if (u->state == 0x80 || (u->state < 0x80 && u->retState == 0x80)) {
                        u->action = 0x7F;
                    }
                    info->unkC = 5000;
                }
                if (info->effects & 0x200) {
                    u->sealS = u->sealS + info->power * (RAND() * 0.2f + 0.9f) * st->kregSeal;
                }
                if (info->effects & 0x400) {
                    u->sealM = u->sealM + info->power * (RAND() * 0.2f + 0.9f) * st->kregSeal;
                }
                if (info->effects & 0x2000) {
                    u->bind = u->bind + info->power * (RAND() * 0.2f + 0.9f) * st->kregOther;
                    u->exeBad = st->exeBad / 2 * (RAND() * 0.8f + 0.2f);
                    u->flags &= 0xFFFFFFFB;
                }
                if (info->effects & 0x23E0) {
                    if (u->comboPartners[0] >= 0) {
                        BtlUnitCancelAction(u);
                    } else if (u->comboLeader >= 0) {
                        BtlUnitCancelAction(&btlUnits[u->comboLeader]);
                    }
                }
            }
            BtlHudStatusIconShow(u, info->effects);
        } else {
            BtlHudStatusIconShow(u, info->effects & 0xF);
        }
    }
    if (info->flags & 0x200) {
        info->effects &= st->status & ~BtlUnitMaskImmuneStatus(u, st->status2);
        st->status &= ~info->effects | 0xF;
        if (info->effects & 0x8000) {
            if (u->id < 4) {
                btlPartyAliveCount++;
                st->status = BtlUnitMaskImmuneStatus(u, st->status2);
                u->powUp = 0;
                u->transp = u->antiMagic = u->regene = u->sleep = u->confu = u->sealS = u->sealM = u->bind = u->vanish = 0;
                for (j = 4; j >= 0; j--) {
                    u->statMod[j] = 0;
                }
                u->wall = 0;
                switch (btlReviveHpMode[u->id]) {
                case 1:
                    st->hp = st->maxHp / 2;
                    break;
                case 2:
                    st->hp = 1;
                    break;
                default:
                    st->hp = st->maxHp;
                    break;
                }
                u->flags = 0;
                u->retPhase = 0;
                u->retState = 0x80;
                BtlUnitSetState(u, 0x40);
                u->subPhase = 1;
                u->frameSpeed = 1.0f;
                u->flags |= 8;
                IpIconSetPos(u->id, 1.5f);
                u->ipSpeed = 0.0f;
                u->ipRet = 1.0f;
                u->ipPos = 1.0f;
                u->ipMode = 1;
                u->scopeType = 0;
                u->flags &= 0xFFFFFF7F;
                if (u->ipSpeed < st->angryRate) {
                    st->angry += (RAND() * 10.0f + 40.0f) * st->angryRate;
                }
                BtlUnitSelectTarget(u, 1, 0x1003);
                if (u->id < 4) {
                    PtyUnitCalcParams(u->params, 0, 0);
                } else {
                    PtyUnitCalcParams(u->params, u->def->data, &ptyBattleParams[u->id - 4]);
                }
            }
        }
        if (info->effects & 0xE0) {
            if (!(st->status & 0xE0)) {
                u->sleep = 0;
            }
        }
        if (info->effects & 0x100) {
            u->confu = 0;
        }
        if (info->effects & 0x200) {
            u->sealS = 0;
        }
        if (info->effects & 0x400) {
            u->sealM = 0;
        }
        if (info->effects & 0x2000) {
            u->bind = 0;
        }
        if (info->effects & 0x4000) {
            u->vanish = 0;
        }
    }
}
