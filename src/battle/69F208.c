/* rodata: 0x006E0C80-0x006E0DA0 */
#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "battle/misc.h"

f32 VuSinAngle(s32);
void BtlCmdListOpen(BtlUnit *, s32);

void BtlCmdMenuSetState(u32 state) {
    BtlCmdMenu *m = &btlCmdMenu;
    BtlCmdRingCam *c;
    BtlUnit *u;
    f32 tgt;
    f32 r;
    s32 a;

    switch (state) {
    case 16:
        SndSePlay3D(0x7DD, 0, 0, NULL);
        m->unkB0 = -1;
        BtlUnitInfoOpen(m->unit, m->hidden[m->unit->id]);
        IpPanelSetMode(4, 16, 1);
        c = &m->ringCam;
        m->unk14 = 2;
        c->angle = m->unk9 * 10922.667f;
        if (m->unk8 != 0) {
            tgt = 45.0f;
            if (m->unk8 > 0) {
                tgt = -55.0f;
            }
        } else {
            tgt = 0.0f;
        }
        c->tilt = tgt;
        c->unk18 = 0x20000;
        c->unk20 = 285.0f;
        c->unk24 = -140.0f;
        c->vel = 0.0f;
        c->dir = 0;
        c->unk28 = 0.0f;
        c->t = 0;
        c->inputTimer = 0;
        BtlChrPartsSetHidden(BtlChrFindPart(btlCmdMenuRingInst, 11), 0);
        MenuCursorShow(180.0f, 190.0f);
        break;
    case 38:
        BtlCmdListOpen(m->unit, 2);
        BtlUnitInfoClose();
        break;
    case 39:
        BtlCmdListOpen(m->unit, 3);
        BtlUnitInfoClose();
        break;
    case 36:
        BtlCmdListOpen(m->unit, 1);
        BtlUnitInfoClose();
        break;
    case 35:
        m->unkB0 = 16;
        BtlUnitInfoClose();
        MenuTacticsOpen((u8 *)m->unit);
        break;
    case 64:
    case 68:
    case 69:
    case 70:
    case 71:
        u = &btlUnits[m->unit->target];
        if (m->targetCount <= 16) {
            BtlUnitInfoOpen(u, m->hidden[u->id]);
            m->unk14 = 3;
        } else {
            MenuCursorHide();
            BtlUnitInfoClose();
            m->unk14 = 8;
        }
        IpPanelSetMode(4, 16, 1);
        m->area.center = &u->pos;
        BtlUnitCollectAreaTargetsAtTarget(m->unit);
        EnvDimStart(m->unit, 32, 0.62f);
        break;
    case 65:
        BtlFormationFindSpot(m->unit, &m->points[1], 0);
        m->unkAC = 0;
        m->unk14 = 0;
        m->points[0].x = m->unit->pos[0];
        m->points[0].z = m->unit->pos[2];
        break;
    case 66:
        MenuCursorHide();
        r = 400.0f;
        BtlUnitInfoClose();
        m->unk14 = 0;
        m->unkAC = 0;
        a = m->unit->rot[1] + 0x8000;
        m->points[0].x = m->unit->pos[0] + VuSinAngle(a) * r;
        m->points[0].z = m->unit->pos[2] + VuCosAngle(a) * r;
        m->targetCount = 0;
        break;
    case 0:
        BtlUnitInfoClose();
        IpGaugeClearActor();
        BtlCmdListClose();
        BtlCmdListReset();
        IpPanelSetMode(4, 16, 1);
        MenuCursorHide();
        if (m->unkB == 0) {
            BtlSchedActDone(m->unit->id);
            m->unit->flags &= 0xFFFFFFF7;
            BtlCamSetMode(1, 0x40, m->unit, 0);
        }
        IpComboMarkSet(0);
        FxComboLinkSet(m->unit, 0);
        if (btlHandoffIpStopOpt == 0) {
            IpTransStart(60, 0.025f);
        } else {
            IpTransStart(0, -0.0625f);
        }
        m->state = 0;
        return;
    }
    m->state = state;
    m->timer = 0;
    if (!(state & 0x40) && m->area.active != 0) {
        m->area.active = 4;
    }
}
