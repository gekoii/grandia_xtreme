#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "sys.h"
#include "field/state.h"
#include "field/event.h"

typedef struct {
    u8 type;
    u8 pad1[3];
    u32 arg[8];
} Ev;

extern void MemSet(void *, s32, s32);

void EvtScnrSysCmdExec(Ev *e) {
    s32 i;
    s32 mask;
    s32 j;
    s32 k;
    ChrObj *p;
    u16 buf[4];

    if (evtScnrWork->cmdCount != 0) {
        for (i = 0; i < evtScnrWork->cmdCount; i++) {
            switch (e->type) {
            case 12:
                switch (e->arg[0]) {
                case 0:
                    MemSet(sysSaveData.localFlags, 0, 0x10);
                    MemSet(sysSaveData.counters, 0, 0x20);
                    break;
                case 1:
                    MemSet(sysSaveData.counters, 0, 0x20);
                    break;
                case 2:
                    MemSet(sysSaveData.localFlags, 0, 0x10);
                    break;
                case 3:
                    MemSet(sysSaveData.monsterFlags, 0, 0x20);
                    break;
                }
                break;
            case 7:
                switch (e->arg[0]) {
                case 0:
                    FldMenuOpen(2);
                    break;
                case 1:
                case 2:
                    break;
                case 3:
                    mask = 0;
                    for (j = 0; j < 4; j++) {
                        k = j + 1;
                        if (e->arg[k] != 0xFF) {
                            mask |= 1 << (e->arg[k] - 1);
                        }
                    }
                    EvtCharSelStart(1, mask);
                    break;
                case 4:
                    EvtCharSelStart(0, 0);
                    break;
                case 5:
                case 6:
                    break;
                case 7:
                    MiniStart(0, e->arg[1] + (e->arg[2] << 8), e->arg[3] + (e->arg[4] << 8));
                    break;
                case 8:
                    MiniStart(1, e->arg[1] + (e->arg[2] << 8), e->arg[3] + (e->arg[4] << 8));
                    break;
                case 9:
                    FldFlagListOpen(e->arg[1] + (e->arg[2] << 8));
                    break;
                case 10:
                    MiniStart(2, e->arg[1] + (e->arg[2] << 8), e->arg[3] + (e->arg[4] << 8));
                    break;
                case 11:
                    sysState.progChangeReq = 1;
                    sysState.unkE = 0;
                    sysState.movieNo = e->arg[1];
                    sysState.unkD = e->arg[2];
                    sysState.nextMapNo = e->arg[3] + (e->arg[4] << 8);
                    sysState.unk16 = e->arg[5] + (e->arg[6] << 8);
                    break;
                case 12:
                    if (sysSaveData.mainFlag >= 0x2AC6) {
                        FldMenuOpen(1);
                    } else {
                        EvtSystemFlagSet(0x8D, 0);
                    }
                    break;
                case 13:
                    sysState.progChangeReq = 1;
                    sysState.movieNo = 8;
                    sysState.unkD = 10;
                    sysState.unkE = 0;
                    sysState.unk13 = e->arg[2];
                    sysState.nextMapNo = EvtGetMapNo();
                    sysState.unk16 = 0x80F1;
                    break;
                case 14:
                    FldMenuOpen(7);
                    break;
                case 15:
                    FldMenuOpen(10);
                    break;
                case 16:
                    FldMenuOpen(12);
                    break;
                case 17:
                    FldMenuOpen(5);
                    break;
                case 18:
                    FldMenuOpen(8);
                    break;
                case 19:
                    FldMenuOpen(9);
                    break;
                case 20:
                    FldMenuOpen(6);
                    break;
                case 21:
                    PtyRestoreAll();
                    p = ChrGetByNo(1);
                    if (p != NULL) {
                        FldHpGaugeInit(&p->hpGauge, sysSaveData.chars[0].hp, sysSaveData.chars[0].maxHp);
                    }
                    break;
                case 22:
                    FldSndBgmPlayNow(e->arg[1]);
                    break;
                case 23:
                    ItemAdd(e->arg[1] + (e->arg[2] << 8));
                    break;
                case 24:
                case 25:
                    sysSoftResetReq = 1;
                    break;
                case 26:
                    FldPictureShow(e->arg[1]);
                    break;
                case 27:
                    ItemKeyRemove(e->arg[1] + (e->arg[2] << 8));
                    break;
                case 28:
                    FldMenuOpen(11);
                    break;
                case 29:
                    FldOverlayImgShow(1);
                    break;
                case 30:
                    FldOverlayImgShow(0);
                    break;
                }
                break;
            case 13:
                switch (e->arg[0]) {
                case 0:
                case 1:
                    break;
                case 2:
                    CamScrStart(e->arg[1]);
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                    break;
                }
                break;
            case 16:
                if (e->arg[1] != 0) {
                    OccMapJumpRequest(evtScnrWork->idBase + e->arg[1], e->arg[2], e->arg[3]);
                } else {
                    OccMapJumpRequest(0, e->arg[2], e->arg[3]);
                }
                break;
            case 15:
                if (e->arg[0] < 3) {
                    for (i = 0; i < 4; i++) {
                        buf[i] = sysSaveData.party[i];
                    }
                    PtyPartySet(buf);
                    PtyPartyGet(buf);
                    for (i = 0; i < 4; i++) {
                        sysSaveData.party[i] = buf[i];
                    }
                }
                break;
            case 20:
                switch (e->arg[1]) {
                case 0:
                    FldSndBgmPlayNow(e->arg[2]);
                    break;
                case 1:
                    break;
                case 2:
                    if (e->arg[0] == 0) {
                        FldSndBgmPlayFade(e->arg[2]);
                        FldSndBgmSetVolume((f32)e->arg[3], 100.0f);
                    } else {
                        FldSndStreamPlay(e->arg[2]);
                    }
                    break;
                case 3:
                    if (e->arg[0] == 0) {
                        FldSndBgmStop();
                    }
                    break;
                case 4:
                    break;
                }
                e++;
                break;
            case 21:
                switch (e->arg[0]) {
                case 0:
                    FldSndBgmSetVolume((f32)e->arg[2], (f32)e->arg[3]);
                    break;
                case 1:
                    break;
                case 2:
                    if (e->arg[1] == 0xFF) {
                        FldSndEmitterEnableAll(e->arg[2]);
                    } else {
                        /* dead store: keeps the call from ending its block (no flow nop) */
                        k = ((s32 (*)(u16, u8))FldSndEmitterEnableById)(e->arg[1], e->arg[2]);
                    }
                    break;
                }
                break;
            case 22:
                break;
            }
        }
        evtScnrWork->cmdCount = 0;
    }
}
