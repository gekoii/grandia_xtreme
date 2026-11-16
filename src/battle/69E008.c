/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "party.h"
#include "battle/misc.h"


void BtlCmdMenuAreaShow(s16, void *, s16);
/* The original never tail-calls; calling through these casts blocks gcc's sibcalls. */
typedef s32 (*F1)(u32);
typedef s32 (*F2)(BtlUnit *, s32, f32);
typedef s64 (*F3)(s32, u8, u8, void *);

void BtlCmdMenuInput(void) {
    BtlCmdMenu *m;
    Pad *pad;
    BtlUnit *cur;
    BtlUnit *u;
    PtyUnit *c;
    PtyItemUse *t;
    PtyMove *mg;
    PtyItem *sk;
    PtyChar *d;
    PtyItemSlot *sl;
    s16 *pid;
    s32 i;
    s32 r;
    s32 st;
    s32 k;
    PtyPartyData *w;
    s32 btn;
    s8 dir;
    s16 id;
    u16 val;
    s16 x;

    m = &btlCmdMenu;
    pad = padPort0;
    cur = m->unit;
    switch (m->state) {
    case 0x10:
        if (pad->repeat & 0x5000) {
            ((F3)SndSePlay3D)(0x7D1, 0, 0, NULL);
            if (pad->repeat & 0x4000) {
                if (++m->unk9 >= 6) {
                    m->unk9 = 0;
                }
                m->ringCam.vel += 10922.667f;
                if (m->ringCam.vel > 65536.0f) {
                    m->ringCam.vel -= 65536.0f;
                }
            } else {
                if (--m->unk9 < 0) {
                    m->unk9 = 5;
                }
                m->ringCam.vel -= 10922.667f;
                if (m->ringCam.vel < -65536.0f) {
                    m->ringCam.vel += 65536.0f;
                }
            }
            if (m->unk8 == 1) {
                m->unk8 = 0;
                m->ringCam.dir = -1;
            } else if (m->unk8 == -1) {
                m->ringCam.dir = 1;
                m->unk8 = 0;
            }
            m->ringCam.inputTimer = 0;
            m->sel = m->unk9;
        } else if (pad->repeat & 0xA000) {
            if (pad->repeat & 0x8000) {
                dir = -1;
                m->ringCam.dir = dir;
                if (m->unk8 == 1) {
                    ((F3)SndSePlay3D)(0x7D1, 0, 0, NULL);
                    m->unk8 = 0;
                    m->sel = m->unk9;
                    m->ringCam.inputTimer = 0;
                } else if (m->unk8 == 0 && BtlUnitCountManaEggs(cur) != 0) {
                    ((F3)SndSePlay3D)(0x7D1, 0, 0, NULL);
                    m->unk8 = dir;
                    m->sel = 6;
                    m->ringCam.inputTimer = 0;
                }
            } else {
                dir = 1;
                m->ringCam.dir = dir;
                if (m->unk8 == -1) {
                    ((F3)SndSePlay3D)(0x7D1, 0, 0, NULL);
                    m->unk8 = 0;
                    m->sel = m->unk9;
                    m->ringCam.inputTimer = 0;
                } else if (m->unk8 == 0) {
                    ((F3)SndSePlay3D)(0x7D1, 0, 0, NULL);
                    m->unk8 = dir;
                    m->sel = 7;
                    m->ringCam.inputTimer = 0;
                }
            }
        }
        if (pad->held & 0x80) {
            if (BtlCmdMenuTargetListBuild(6, 4) == 0) {
                return;
            }
            cur->scopeType = 0x301;
            ((F1)BtlCmdMenuSetState)(0x4F);
            BtlUnitInfoOpen(&btlUnits[cur->target], m->hidden[cur->target]);
            m->unkB0 = -1;
            BtlUnitCollectAreaTargetsAtTarget(m->unit);
            ((F2)EnvDimStart)(m->unit, 0x20, 0.62f);
            return;
        }
        if (pad->held & 0x10) {
            if (m->flags & 8) {
                ((F3)SndSePlay3D)(0x7D4, 0, 0, NULL);
                return;
            }
            m->unk8 = 0;
            m->unk9 = 3;
            m->sel = 3;
            ((F1)BtlCmdMenuSetState)(0x23);
            ((F3)SndSePlay3D)(0x7D2, 0, 0, NULL);
            m->unkB0 = 0x10;
            return;
        }
        if (pad->trigger & 0x20) {
            if ((m->flags >> m->sel) & 1) {
                ((F3)SndSePlay3D)(0x7D4, 0, 0, NULL);
                return;
            }
            switch (m->sel) {
            case 0:
                if (BtlCmdMenuTargetListBuild(2, 2) == 0) {
                    return;
                }
                cur->action = 0;
                cur->scopeRadius = 0.0f;
                cur->skillId = cur->def->id - 0xF4C;
                cur->skillLevel = 1;
                cur->scopeType = 0x201;
                ((F1)BtlCmdMenuSetState)(0x40);
                break;
            case 5:
                if (BtlCmdMenuTargetListBuild(2, 2) == 0) {
                    return;
                }
                cur->action = 1;
                c = cur->params;
                cur->skillId = cur->def->id - 0xF42;
                cur->skillLevel = 1;
                cur->scopeRadius = 0.0f;
                if (c->bitCsf & 0x20000) {
                    cur->scopeType = 0x202;
                    if (c->bitCsf & 0x10000) {
                        r = 2200;
                        m->area.size = r;
                        cur->scopeRadius = 2200.0f;
                        BtlCmdMenuAreaShow(cur->scopeType, &btlUnits[cur->target].pos, 600);
                    } else {
                        r = 600;
                        m->area.size = r;
                        cur->scopeRadius = 600.0f;
                        BtlCmdMenuAreaShow(cur->scopeType, &btlUnits[cur->target].pos, 0);
                    }
                } else if (c->bitCsf & 0x10000) {
                    m->area.size = 250;
                    cur->scopeType = 0x203;
                    cur->scopeRadius = 250.0f;
                    BtlCmdMenuAreaShow(cur->scopeType, &btlUnits[cur->target].pos, 0);
                } else {
                    cur->scopeType = 0x201;
                }
                ((F1)BtlCmdMenuSetState)(0x45);
                break;
            case 4:
                ((F1)BtlCmdMenuSetState)(0x24);
                break;
            case 6:
                ((F1)BtlCmdMenuSetState)(0x26);
                break;
            case 7:
                ((F1)BtlCmdMenuSetState)(0x27);
                break;
            case 1:
                ((F1)BtlCmdMenuSetState)(0x41);
                break;
            case 3:
                ((F1)BtlCmdMenuSetState)(0x23);
                break;
            case 2:
                ((F1)BtlCmdMenuSetState)(0x42);
                break;
            }
            ((F3)SndSePlay3D)(0x7D2, 0, 0, NULL);
            m->unkB0 = 0x10;
            return;
        }
        if (!(pad->trigger & 4)) {
            return;
        }
        if (SysIsDebugMode() == 0) {
            return;
        }
        if (pad->held & 1) {
            u = btlEnemyUnits;
            for (i = 11; i >= 0; i--, u++) {
                u->attacker = cur->id;
                BtlHitQueueAddDamage(u, 99999999, 0.0f, u->rot[1], 0x7800, 0);
            }
        }
        cur->ipRet = 1.0f;
        cur->ipMode = 1;
        cur->ipPos = 1.0f;
        cur->ipSpeed = 0.0f;
        cur->scopeType = 0;
        cur->flags &= 0xFFFFFF7F;
        cur->action = 0x7F;
        BtlUnitSetState(cur, 0x80);
        ((F1)BtlCmdMenuSetState)(0);
        return;
    case 0x4F:
        BtlCmdMenuTargetInput();
        if (pad->held & 0x80) {
            return;
        }
        ((F1)BtlCmdMenuSetState)(0x10);
        ((F2)EnvDimStart)(NULL, 0x20, 1.0f);
        return;
    case 0x24:
    case 0x26:
    case 0x27:
        switch (BtlCmdListInput()) {
        case -1:
            ((F1)BtlCmdMenuSetState)(0x10);
            return;
        case 2:
            cur->action = 0x7F;
            BtlCmdMenuTargetListBuild(3, 3);
            ((F1)BtlCmdMenuSetState)(0x44);
            return;
        case 1:
            t = ptyItemTable[cur->skillId].use;
            if (BtlCmdMenuTargetListBuild(t->kind, t->unk1) != 0) {
                cur->skillLevel = 1;
                cur->action = 2;
                m->area.size = t->unkC;
                cur->scopeRadius = m->area.size;
                BtlUnitSetTargetArea(cur, t->target, t->unk1);
                BtlCmdMenuAreaShow(cur->scopeType, &btlUnits[cur->target].pos, 0);
                ((F1)BtlCmdMenuSetState)(0x44);
            } else {
                ((F1)BtlCmdMenuSetState)(0x24);
            }
            return;
        case 3:
            mg = &ptyMoveTable[cur->skillId];
            if (BtlCmdMenuTargetListBuild(mg->kind, mg->unk1) != 0) {
                cur->action = 3;
                cur->skillLevel = 1;
                m->area.size = BtlSpellGetLevelData(cur, 0, 0x10)->scope;
                cur->scopeRadius = m->area.size;
                BtlUnitSetTargetArea(cur, mg->target, mg->unk1);
                x = BtlSpellGetLevelData(cur, 0, 0)->scope;
                BtlCmdMenuAreaShow(cur->scopeType, &btlUnits[cur->target].pos, x == m->area.size ? 0 : x);
                ((F1)BtlCmdMenuSetState)(0x46);
            } else {
                ((F1)BtlCmdMenuSetState)(0x26);
            }
            return;
        case 4:
            mg = &ptyMoveTable[cur->skillId];
            if (BtlCmdMenuTargetListBuild(mg->kind, mg->unk1) != 0) {
                cur->action = 4;
                /* levels read as an s32 (alias set): the plain field schedules differently */
                st = (u16)((PtyMoveLevelData *)*(s32 *)&mg->levels)[cur->skillLevel - 1].scope;
                m->area.size = st;
                cur->scopeRadius = m->area.size;
                BtlUnitSetTargetArea(cur, mg->target, mg->unk1);
                BtlCmdMenuAreaShow(cur->scopeType, &btlUnits[cur->target].pos, 0);
                ((F1)BtlCmdMenuSetState)(0x47);
            } else {
                ((F1)BtlCmdMenuSetState)(0x27);
            }
            return;
        }
        return;
    case 0x23:
        r = MenuTacticsUpdate();
        switch (r) {
        case -1:
            ((F1)BtlCmdMenuSetState)(0x10);
            break;
        case 1:
            if (btlAutoTactics != 0 && cur->ai != 0) {
                cur->action = 0x7F;
                AiPartyChooseCommand(cur);
                ((F1)BtlCmdMenuSetState)(0);
            } else {
                ((F1)BtlCmdMenuSetState)(0x10);
            }
            break;
        }
        break;
    case 0x40:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
        BtlCmdMenuTargetInput();
        btn = pad->trigger;
        if (btn & 0x40) {
            ((F3)SndSePlay3D)(0x7D3, 0, 0, NULL);
            if (m->targetCount > 0x10) {
                MenuCursorShow(0.0f, 0.0f);
            }
            switch (m->state) {
            case 0x40:
            case 0x45:
                ((F1)BtlCmdMenuSetState)(0x10);
                break;
            case 0x46:
                m->unk14 = 2;
                ((F1)BtlCmdMenuSetState)(0x26);
                break;
            case 0x47:
                m->unk14 = 2;
                ((F1)BtlCmdMenuSetState)(0x27);
                break;
            case 0x44:
                m->unk14 = 2;
                ((F1)BtlCmdMenuSetState)(0x24);
                break;
            }
            ((F2)EnvDimStart)(NULL, 0x20, 1.0f);
            return;
        }
        if (!(btn & 0x20)) {
            return;
        }
        ((F3)SndSePlay3D)(0x7D2, 0, 0, NULL);
        if (cur->action == 0x7F) {
            w = ptyPartyData;
            sl = w->bag;
            pid = &sl[cur->skillId].id;
            id = *pid;
            sk = &ptyItemTable[id];
            d = &ptyCharData[cur->params->id - 0x1001];
            val = d->equip[sk->type - 5];
            *pid = val;
            w->bag[cur->skillId].count = (s16)val > 0;
            d->equip[sk->type - 5] = id;
            cur->skillId = id;
            BtlUnitSetState(cur, 0x86);
            m->unkB = 1;
        } else {
            btn = st = m->state;
            if (btn == 0x40 && !(cur->params->status & 0x200)) {
                BtlCmdTryLearnMove(cur); btn = m->state;
            } else if (st != 0x47) {
                goto skip; /* folded into the else-if, the bnel to skip becomes a bne */
            } else if (m->enabledMask != 0) {
                BtlCmdTryLearnCombo(cur); btn = m->state;
            }
            if (btn == 0x47 && cur->skillId >= 0xDD) {
                BtlCmdComboLinkPartners(cur);
            }
        skip:
            BtlUnitSetState(cur, 0xC0);
        }
        ((F1)BtlCmdMenuSetState)(0);
        ((F2)EnvDimStart)(NULL, 0x20, 1.0f);
        return;
    case 0x41:
        if (++m->timer >= 0x30) {
            m->timer = 0;
        }
        if (pad->repeat & 0x2000) {
            ((F3)SndSePlay3D)(0x7D1, 0, 0, NULL);
            if (--m->unkAC < 0) {
                m->unkAC = 5;
            }
            m->unk14 = 0;
        }
        if (pad->repeat & 0x8000) {
            ((F3)SndSePlay3D)(0x7D1, 0, 0, NULL);
            if (++m->unkAC >= 6) {
                m->unkAC = 0;
            }
            m->unk14 = 0;
        }
        btn = pad->trigger;
        if (btn & 0x40) {
            ((F3)SndSePlay3D)(0x7D3, 0, 0, NULL);
            ((F1)BtlCmdMenuSetState)(0x10);
            return;
        }
        if (!(btn & 0x20)) {
            return;
        }
        ((F3)SndSePlay3D)(0x7D2, 0, 0, NULL);
        cur->action = 0x7F;
        btn = m->unkAC;
        if (btn == 0) {
            BtlUnitSetState(cur, 0x84);
        } else {
            cur->moveTarget[0] = m->points[btn].x;
            cur->moveTarget[2] = m->points[btn].z;
            cur->ipPos = cur->ipRet = cur->params->restartEvade;
            cur->scopeType = 0;
            cur->ipMode = 1;
            cur->flags = (cur->flags & 0xFFFFFF7F) | 0x10;
            cur->ipSpeed = 0.0f;
            BtlUnitSetState(cur, 0x80);
        }
        ((F1)BtlCmdMenuSetState)(0);
        return;
    case 0x42:
        btn = pad->trigger;
        if (btn & 0x40) {
            MenuCursorShow(0.0f, 0.0f);
            ((F3)SndSePlay3D)(0x7D3, 0, 0, NULL);
            ((F1)BtlCmdMenuSetState)(0x10);
            return;
        }
        if (btn & 0x20) {
            ((F3)SndSePlay3D)(0x7D2, 0, 0, NULL);
            k = 1;
            cur->action = 5;
            cur->skillId = cur->def->id - 0xF38;
            cur->skillLevel = k;
            /* a fourth argument BtlUnitSelectTarget doesn't read (see its definition) */
            ((void (*)(BtlUnit *, s32, s32, s32))BtlUnitSelectTarget)(cur, 1, 0x1003, 1);
            cur->scopeType = 0x201;
            BtlUnitSetState(cur, 0xC0);
            ((F1)BtlCmdMenuSetState)(0);
        }
        return;
    }
}
