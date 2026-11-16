/* cflags: -fno-strength-reduce */
/* -fno-strength-reduce: the comboPartners loops index (with it on, they walk pointers) */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"

#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))

void VuVecCopy(void *, void *);

s32 BtlUnitCommandValidate(BtlUnit *arg0) {
    BtlAction *p;
    PtyMoveLevelData *act;
    PtyUnit *st;
    s8 *t;
    PtyMove *e;
    PtyMove *e4;
    s32 kind;
    s32 side;
    s32 res;
    s32 i;
    s32 c;
    s32 one;
    s32 c2;

    one = 0;
    res = 1;
    if (arg0->state < 0x40) {
        return 0;
    }
    side = arg0->ipMode;
    if (side != 2) {
        return 0;
    }
    if (arg0->def->validate != NULL) {
        arg0->def->validate(arg0);
        p = BTL_UNIT_ACT(arg0);
        arg0->skillId = p->moveId;
        act = p->level;
        arg0->scopeRadius = act->scope;
        arg0->scopeType = p->scopeType;
        switch (p->scopeMode) {
        case 2:
            arg0->scopeType |= 0x100;
            break;
        case 4:
            arg0->scopeType |= 0x100;
        default:
            arg0->scopeType |= 0x200;
            break;
        }
    }
    st = btlUnits[arg0->target].params;
    if (((st->status & 1) && arg0->target != arg0->id) || (st->bitCsf & 8)) {
        res = 0;
    } else {
        if (arg0->comboPartners[0] >= 0) {
            for (i = 0; i < 3; i++) {
                if (arg0->comboPartners[i] >= 0 && (btlUnits[arg0->comboPartners[i]].ipPos > 0.0f || btlUnits[arg0->comboPartners[i]].state != 0xC0)) {
                    break;
                }
            }
            if (i < 3) {
                if (btlActingUnit == arg0->id || btlActingUnit == arg0->body) {
                    for (i = 0; i < 3; i++) {
                        c = arg0->comboPartners[i];
                        if (c >= 0) {
                            btlUnits[c].flags |= 8;
                        }
                    }
                }
                return 0;
            }
        }
        if (arg0->id < 4) {
            kind = 6;
            switch (arg0->action) {
            case 0:
            case 1:
                res = 2;
                kind = 2;
                break;
            case 2:
                t = ptyItemTable[arg0->skillId].use;
                if ((*(s32 *)t & 0xFFFF00) != 0x10500 || (btlUnits[arg0->target].params->status & 0x8000)) {
                    res = t[1];
                } else {
                    res = 0;
                }
                kind = t[0];
                one = 1;
                break;
            case 3:
                e = &ptyMoveTable[arg0->skillId];
                if ((arg0->params->status & 0x400) || ((*(s32 *)e & 0xFFFF00) == 0x10500 && !(btlUnits[arg0->target].params->status & 0x8000))) {
                    res = 0;
                } else {
                    res = e->unk1;
                }
                kind = e->kind;
                one = 1;
                break;
            case 4:
                e4 = &ptyMoveTable[arg0->skillId];
                if (!(arg0->params->status & 0x200)) {
                    res = e4->unk1;
                } else {
                    res = 0;
                }
                kind = e4->kind;
                one = 1;
                break;
            case 5:
                res = 2;
                one = 1;
                break;
            }
            if (res != 0) {
                switch (res) {
                case 2:
                    res = 0x200;
                    if (arg0->target == arg0->id && kind == 2) {
                        res = 0;
                    }
                    break;
                case 4:
                    res = 0x300;
                    break;
                default:
                    res = 0x100;
                    break;
                }
            }
        } else {
            p = BTL_UNIT_ACT(arg0);
            act = p->level;
            res = p->scopeMode;
            if ((act->mpCost > 0 && (CEIL(act->mpCost * arg0->params->mgcMpRate) > (f32)arg0->params->mp || (arg0->params->status & 0x400))) ||
                (act->cost > 0 && (CEIL(act->cost * arg0->params->spcSpRate) > (f32)arg0->params->sp || (arg0->params->status & 0x200)))) {
                res = 0;
            }
            one = p->unk4 == 0;
            if (res != 0) {
                switch (res) {
                case 2:
                    res = 0x100;
                    if (arg0->target == arg0->id) {
                        if (p->targetSide == 2) {
                            res = 0;
                        }
                    } else {
                        c2 = btlUnits[arg0->target].body;
                        if (c2 >= 0) {
                            if (arg0->id == c2 || arg0->body == c2) {
                                res = 0;
                            }
                        } else if (arg0->body == arg0->target) {
                            res = 0;
                        }
                    }
                    break;
                case 4:
                    res = 0x300;
                    break;
                default:
                    res = 0x200;
                    break;
                }
            }
        }
        if (!(arg0->params->status & 0x100) && !(arg0->scopeType & res)) {
            res = 0;
        }
        if ((arg0->params->status & 0x1000) && btlActingUnit == 0xFF && (rand() >> 15) <= 0x7FFF) {
            BtlHudMarkerSpawn(arg0, 6);
            res = 0;
            FxFailLabelShow(arg0, 1);
        }
    }
    if (res <= 0) {
        arg0->ipMode = 1;
        arg0->ipPos = arg0->ipRet;
        arg0->ipSpeed = 0.0f;
        arg0->scopeType = 0;
        arg0->flags &= 0xFFFFFF7F;
        BtlUnitSetState(arg0, 0x80);
        arg0->action = 0x7F;
        VuVecCopy(arg0->moveTarget, arg0->pos);
        if (arg0->comboPartners[0] >= 0) {
            BtlUnitCancelAction(arg0);
        }
        return 0;
    }
    return one != 0 ? 2 : 1;
}
