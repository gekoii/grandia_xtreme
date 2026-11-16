#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"





void SysDemoUpdate(void) {
    if (sysDemo.mode == 0) {
        return;
    }
    SndBgmSetEnabled(1);
    SndSeSetRequestEnabled(0);
    SndSeSuspend();
    PadSetVibEnabled(0);
    if (sysDemo.mode == 30 && (padPort0Trigger & 0x800)) {
        SndSeResume();
        SndSeSetRequestEnabled(1);
        SndSePlay(2002, 0, 0, 30, 0);
        sysDemo.mode = 32;
    }
    if (sysDemo.mode >= 2) if (sysDemo.mode < 6) {
        if (SndBgmGetState(0) == 0 && SndBgmGetState(1) == 0) {
            sysDemo.mode = 26;
        }
        if (sysDemo.unk44 == 0 && GphFadeIsActive(&sysDemo.fade) == 0 &&
            SndSeBankLoadGetState() == -1 && FileGetReadState() == -1 && (padPort0Trigger & 0x800)) {
            SndBgmFadeOut(0, 150);
            SndBgmFadeOut(1, 150);
            sysDemo.mode = 26;
            sysDemo.skipped = 1;
        }
    }
    switch (sysDemo.mode) {
    case 1:
        if (sysState.progChangeReq == 0) {
            sysDemo.mode = 14;
            GphFadeSetColor(&sysDemo.fade, 0.0f, 0.0f, 0.0f, 128.0f);
            sysDemo.timer = 0;
        }
        break;
    case 2:
        GphScreenFadeSetColor(0.0f, 0.0f, 0.0f, 0.0f);
        if (sysDemo.ready == 1) {
            sysDemo.timer = 0;
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 128.0f, 60.0f);
            sysDemo.mode++;
        }
        break;
    case 3:
        if (GphFadeIsActive(&sysDemo.fade) == 0) {
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f);
            sysDemo.timer = 0;
            sysDemo.mode++;
        }
        break;
    case 4:
        if (++sysDemo.timer >= 1200) {
            sysDemo.timer = 0;
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 128.0f, 60.0f);
            sysDemo.mode++;
        }
        break;
    case 5:
        if (GphFadeIsActive(&sysDemo.fade) == 0 && FileGetReadState() == -1 && SndSeBankLoadGetState() == -1) {
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f);
            SysProgRequest(0, 2);
            sysDemo.mode = 2;
            sysDemo.timer = 0;
            sysDemo.ready = 0;
        }
        break;
    case 6:
        if (GphFadeIsActive(&sysDemo.fade) == 0) {
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 128.0f, 30.0f);
            sysDemo.mode = 7;
            SndBgmFadeOut(0, 140);
            SndBgmFadeOut(1, 140);
        }
        break;
    case 7:
        if (GphFadeIsActive(&sysDemo.fade) == 0) {
            sysDemo.mode = 8;
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 0.0f, 30.0f);
            SysProgRequest(0, 2);
        }
        break;
    case 8:
        if (GphFadeIsActive(&sysDemo.fade) == 0) {
            SndBgmFadeOut(0, 150);
            SndBgmFadeOut(1, 150);
            sysDemo.mode = 9;
        }
        break;
    case 9:
        if (sysDemo.ready == 1) {
            sysDemo.mode = 13;
        }
        break;
    case 10:
        GphFadeSetColor(&sysDemo.fade, 0.0f, 0.0f, 0.0f, 0.0f);
        GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 128.0f, 30.0f);
        sysDemo.mode = 11;
        break;
    case 11:
        if (GphFadeIsActive(&sysDemo.fade) == 0 && SndBgmGetState(0) == 0 && SndBgmGetState(1) == 0 &&
            FileGetReadState() == -1 && SndSeBankLoadGetState() == -1) {
            sysDemo.mode = 0;
            sysState.nextMapNo = 0xD000;
            sysState.movieNo = 1;
            sysState.progChangeReq = 1;
            sysState.unkD = 1;
            return;
        }
        break;
    case 12:
        if (sysState.progChangeReq == 0) {
            SysDemoReset();
            return;
        }
        break;
    case 13:
        return;
    case 14:
        if (++sysDemo.timer >= 240) {
            sysDemo.mode = 2;
            sysDemo.timer = 0;
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f);
            SndBgmPlay(0x382, 0);
        }
        break;
    case 26:
        if (GphFadeIsActive(&sysDemo.fade) == 0) {
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 128.0f, 30.0f);
            sysDemo.ready = 0;
            sysDemo.mode++;
        }
        break;
    case 27:
        if (GphFadeIsActive(&sysDemo.fade) == 0) {
            sysDemo.mode++;
            SysProgRequest(0, 2);
            sysDemo.timer = 0;
            sysDemo.promptAlpha = 0;
            sysDemo.captionAlpha0 = 0;
            sysDemo.captionAlpha1 = 0;
        }
        break;
    case 28:
        GphScreenFadeSetColor(0.0f, 0.0f, 0.0f, 0.0f);
        if (sysDemo.ready == 1) {
            GphFadeSetColor(&sysDemo.fade, 0.0f, 0.0f, 0.0f, 0.0f);
            sysDemo.mode++;
        }
        break;
    case 29:
        if (++sysDemo.timer == 1) {
            if (sysDemo.skipped != 0) {
                SndBgmPlay(0x372, 0);
            } else {
                SndBgmPlay(0x376, 0);
            }
        }
        if (sysDemo.skipped != 0 || sysDemo.timer == 360) {
            sysDemo.mode++;
        }
        break;
    case 30:
        SndSeResume();
        SndSeSetRequestEnabled(1);
        if (++sysDemo.timer == 3000) {
            GphFadeStart(&sysDemo.fade, 0, 0.0f, 0.0f, 0.0f, 128.0f, 30.0f);
            sysDemo.mode++;
        }
        break;
    case 31:
        if (GphFadeIsActive(&sysDemo.fade) == 0) {
            SysDemoStart(1);
            GphFadeSetColor(&sysDemo.fade, 0.0f, 0.0f, 0.0f, 128.0f);
        }
        break;
    case 32:
        PadSetVibEnabled(1);
        SndSeResume();
        SndSeSetRequestEnabled(1);
        return;
    case 99:
        if (sysDemo.ready == 1) {
            SysDemoStart(1);
        }
        break;
    }
    GphFadeUpdate(&sysDemo.fade);
    GphFadeUpdate(&sysDemo.fade2);
    SndBgmSetEnabled(0);
}
