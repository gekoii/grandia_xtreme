/* rodata: 0x006E0E80-0x006E0F84 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"

extern u8 btlStrForestall[];
extern u8 btlStrSurprised[];

void BtlStartUpdate(void) {
    BtlHandoff *st;
    BtlCharDef *s;
    BtlUnit *u;
    s32 i;
    s32 j;
    s32 k;
    s32 v;
    BtlUnit *w;
    s32 r;

    switch (btlStartState) {
    case 2:
        break;
    case 0:
        GphScreenFadeSetColor(255.0f, 0.0f, 0.0f, 0.0f);
        GphScreenFadeStart(0, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f);
        BtlCamSetMode(0, 0, 0, BtlStartCamDefault);
        st = &btlHandoff;
        btlBgmHandle.w = SndBgmPlay(st->startType, st->unk60);
        if (st->debugStart == 1) {
            BtlCamSetMode(7, 0, 0, NULL);
        } else if (btlEncounterFlags & 0x10) {
            if (btlEncounterFlags == 0x1A) {
                BtlCamSetMode(0, 0, 0, BtlStartCamBossLong);
            } else {
                BtlCamSetMode(0, 0, 0, BtlStartCamBoss);
            }
        } else if (btlEncounterFlags != 0) {
            BtlCamSetMode(0, 0, 0, BtlStartCamSpecial);
        } else if (st->encType == 2) {
            BtlCamSetMode(0, 0, 0, BtlStartCamSurprised);
        } else if (st->encType == 1) {
            BtlCamSetMode(0, 0, 0, BtlStartCamForestall);
        }
        btlStartTimer = 0;
        btlStartState++;
        return;
    case 1:
        btlStartTimer++;
        if (btlStartTimer == 2) {
            if (btlHandoff.encType == btlStartState) {
                BtlCaptionShow(btlStrForestall, 0xB4, NULL);
            } else if (btlHandoff.encType == btlStartTimer) {
                BtlCaptionShow(btlStrSurprised, 0xB4, NULL);
            }
            BtlHudSetVisible(1);
            IpPanelSetMode(4, 0x10, 1);
            if (btlHandoff.debugStart == 1) {
                BtlCamSetMode(1, 0x40, 0, NULL);
            }
        }
        if (btlEncounterFlags == 0x1A) {
            if (btlStartTimer == 2) {
                EnvDimStart(0, 400, 1.0f);
            }
            if (btlStartTimer >= 600) {
                btlStartTimer = 0;
                btlStartState++;
            }
        } else if (btlStartTimer >= 60) {
            btlStartTimer = 0;
            btlStartState++;
        }
        break;
    }
    if (BtlCharDefsFinishStep() != 0 && btlStartState == 2) {
        btlPhase = 0x80;
        IpGaugeSetShown(1);
        for (i = 0, s = btlCharDefs; i < 4; i++, s++) {
            s->type = 0;
            s->unk6 = 0;
        }
        for (i = 4; i < 8; i++, s++) {
            if (s->id != 0) {
                switch (s->id & 0xFF) {
                case 1:
                    s->type = (s->id < 0x200) ? 2 : 3;
                    break;
                case 6:
                case 0x1E:
                    s->type = 4;
                    break;
                case 8:
                    s->type = 5;
                    break;
                case 0xD:
                case 0x41:
                    s->type = 0;
                    break;
                case 0x29:
                    s->type = 6;
                    break;
                default:
                    s->type = 1;
                    break;
                }
                r = BtlMotionKeyCount(s->motionEvents[10], 1);
                if (r >= 2) {
                    s->unk6 = r - 1;
                } else {
                    s->unk6 = 0;
                }
            }
        }
        u = btlUnits;
        for (j = 15; j >= 0; j--, u++) {
            u->ipPos -= u->params->startIpOfs;
        }
        u = btlUnits;
        switch (btlHandoff.encType) {
        case 1:
            u += 4;
            for (j = 11; j >= 0; j--, u++) {
                u->ipPos += 0.5f;
            }
            break;
        case 2:
            for (j = 3; j >= 0; j--, u++) {
                u->ipPos += 0.5f;
            }
            break;
        }
        v = 0x7F;
        w = btlEnemyUnits;
        for (k = 11; k >= 0; k--, w++) {
            w->action = v;
        }
        BtlStartApplyStatus();
    }
}
