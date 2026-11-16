/* rodata: 0x00675950-0x00675958 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "sys.h"
#include "field/state.h"
#include "field/n25.h"


void FldTitleUpdate(void) {
    FldTitleWork *e;
    s32 n;
    s32 r;
    u16 btn;

    if (fldTitleWork->started == 1) {
        if (fldTitleWork->cursor == 0 && GphFadeIsActive(fldFades) == 0) {
            sysState.nextMapNo = 0xD000;
            sysState.movieNo = 1;
            sysState.progChangeReq = 1;
            sysState.unkD = 1;
        }
        return;
    }
    if (fldState.mode != FLD_MODE_TRANSITION) {
        return;
    }
    if (SysDemoGetMode() == 0x63) {
        return;
    }
    if (SysDemoGetMode() == 1) {
        return;
    }
    if (SysDemoGetMode() == 0xE) {
        return;
    }
    if (SysDemoGetMode() == 0x1C && MapLoadGetState() == -1 && EvtGetMapNo() != 0x1820 && EvtGetMapNo() != 0x1830) {
        if (SysDemoIsSkipped()) {
            MapJumpRequest(0x1830, 0);
        } else {
            MapJumpRequest(0x1820, 0);
        }
    }
    r = EvtGetMapNo();
    if (r == 0x1820 || r == 0x1830) {
        FldTitleCamFixedUpdate();
    } else {
        FldTitleCamFlyUpdate();
    }
    if (SysDemoGetMode() == 0x20) {
        btn = padPort0[0].trigger;
        if (btn & 0x1000) {
            if (fldTitleWork->cursor == 0) {
                fldTitleWork->cursor = 2;
            } else {
                fldTitleWork->cursor--;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            return;
        }
        if (btn & 0x4000) {
            if (fldTitleWork->cursor < 2) {
                fldTitleWork->cursor++;
            } else {
                fldTitleWork->cursor = 0;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            return;
        }
        if (btn & 0x20) {
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            switch (fldTitleWork->cursor) {
            case 0:
                SysDemoSetMode(0);
                fldTitleWork->started = 1;
                sysNewGameFlag = 1;
                GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 128.0f, 60.0f);
                return;
            case 1:
                FldMenuOpen(0xE);
                return;
            case 2:
                FldMenuOpen(0xD);
                return;
            }
            return;
        }
    }
    if (SysDemoGetMode() < 6) {
        e = fldTitleWork;
        if (!(e->t < 0.94)) {
            SysDemoSetReady();
            e = fldTitleWork;
        }
        if (e->shot == e->shotNext) {
            e->shotNext = e->shotNext + 1;
            n = ChrTenantGetCount();
            do {
                fldState.unk2D = VuRandInt(n) + 1;
            } while (!(ChrTenantAt(fldState.unk2D - 1)->flags & CHR_FLAG_ENCOUNTER));
        }
    }
    if (fldTitleWork->shot >= 7) {
        fldTitleWork->shot = 0;
        fldTitleWork->shotNext = 1;
    }
}
