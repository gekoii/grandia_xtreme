#include "common.h"
#include "save.h"
#include "sys.h"
#include "protos/main.h"
#include "party.h"

void MemSet(void *, s32, s32);

void SysIniLoad(SysState *arg0) {
    char buf[256];
    char val[256];
    char *p;
    s32 len;
    s32 i;
    s32 j;
    s32 k;
    s32 match;
    s32 end;

    MemSet(&sysIni, 0, 0x2E0);
    if (FileGetSizeAligned(sysIniFileName) == 0) {
        arg0->frameSkip = 1;
        arg0->program = 1;
        arg0->frameMode = 0;
        arg0->debugMode = 0;
        sysIni.motionSmooth = 10.0f;
        sysIni.collision0 = 0xFF;
        sysIni.useMdtData = 1;
        PadSetVibEnabled(0);
        return;
    }
    FileRead(sysIniFileName, 0x100000);
    p = (char *)0x100000;
    end = 0;
    for (;;) {
        MemSet(buf, 0, 0x100);
        len = 0;
        for (;;) {
            if (*p == 0) {
                return;
            }
            if (*p == '\r') {
                p += 2;
                break;
            }
            buf[len] = *p;
            p++;
            len++;
            if (len >= 0x100) {
                return;
            }
        }

        for (i = 0; i < len; i++) {
            if (buf[i] == ';') {
                buf[i] = 0;
                len = i;
                break;
            }
        }
        if (buf[0] == '[' && buf[1] == 'e' && buf[2] == 'n' && buf[3] == 'd' && buf[4] == ']') {
            end = 1;
        }
        if (end == 1) {
            break;
        }
        if (len != 0) {
            for (i = 0; i < 150; i++) {
                match = 1;
                for (j = 0; j < 10; j++) {
                    if (buf[j] == 0 || buf[j] != sysIniKeyTable[i][j]) {
                        match = 0;
                        break;
                    }
                }
                if (match == 1) {
                    MemSet(val, 0, 0x100);
                    k = 0;
                    for (j = 10; j < len; j++) {
                        if (buf[j] != '\t') {
                            val[k++] = buf[j];
                        }
                    }
                    switch (i) {
                    case 0:
                        arg0->program = atoi(val);
                        break;
                    case 2:
                        arg0->frameMode = atoi(val);
                        break;
                    case 1:
                        if (val[0] == 'y') {
                            arg0->debugMode = 1;
                        }
                        break;
                    case 3:
                        if (val[0] == 'y') {
                            arg0->frameSkip = 1;
                        }
                        break;
                    case 4: case 5: case 6: case 7: case 8: case 9:
                        sysIni.backColor[i - 4] = atoi(val);
                        break;
                    case 10:
                        sysIni.fogDist = atoi(val);
                        break;
                    case 11:
                        sysIni.clipFar = atoi(val);
                        break;
                    case 12:
                        sysIni.clipNear = atoi(val);
                        break;
                    case 13:
                        sysIni.viewAngle = atoi(val);
                        break;
                    case 14: case 15: case 16: case 17:
                        sysIni.ambient[i - 14] = atoi(val);
                        break;
                    case 18: case 19: case 20: case 21:
                        sysIni.light0Color[i - 18] = atoi(val);
                        break;
                    case 22: case 23:
                        sysIni.light0Angle[i - 22] = atoi(val);
                        break;
                    case 24: case 25: case 26: case 27:
                        sysIni.light1Color[i - 24] = atoi(val);
                        break;
                    case 28: case 29:
                        sysIni.light1Angle[i - 28] = atoi(val);
                        break;
                    case 30:
                        sysIni.motionSpeed = atof(val);
                        break;
                    case 31:
                        sysIni.motionSmooth = atof(val);
                        break;
                    case 32:
                        if (val[0] == 'y') {
                            sysIni.collision0 = 0xFF;
                        } else {
                            sysIni.collision0 = 0;
                        }
                        break;
                    case 33:
                        if (val[0] == 'y') {
                            sysIni.viewerNoLight = 0;
                        } else {
                            sysIni.viewerNoLight = 1;
                        }
                        break;
                    case 34:
                        if (val[0] == 'y') {
                            sysIni.viewerCrs = 1;
                        } else {
                            sysIni.viewerCrs = 0;
                        }
                        break;
                    case 35:
                        if (val[0] == 'y') {
                            sysIni.viewerSdw = 1;
                        } else {
                            sysIni.viewerSdw = 0;
                        }
                        break;
                    case 36:
                        if (val[0] == 'y') {
                            sysIni.viewerStg = 1;
                        } else {
                            sysIni.viewerStg = 0;
                        }
                        break;
                    case 37:
                        if (val[0] == 'y') {
                            sysIni.ground0Dsp = 1;
                        } else {
                            sysIni.ground0Dsp = 0;
                        }
                        break;
                    case 38:
                        if (val[0] == 'y') {
                            sysIni.viewerNoLoop = 0;
                        } else {
                            sysIni.viewerNoLoop = 1;
                        }
                        break;
                    case 39:
                        if (val[0] == 'y') {
                            sysIni.viewerPtc = 1;
                        } else {
                            sysIni.viewerPtc = 0;
                        }
                        break;
                    case 40:
                        sysIni.walkSpeed0 = atof(val);
                        break;
                    case 41:
                        if (val[0] == 'y') {
                            sysIni.useMdtData = 1;
                        } else {
                            sysIni.useMdtData = 0;
                        }
                        break;
                    case 42:
                        sysIni.cameraAi0 = atoi(val);
                        break;
                    case 43:
                        if (val[0] == 'y') {
                            sysIni.encountOn = 1;
                        } else {
                            sysIni.encountOn = 0;
                        }
                        break;
                    case 44:
                        if (val[0] == 'y') {
                            PadSetVibEnabled(1);
                        } else {
                            PadSetVibEnabled(0);
                        }
                        break;
                    case 45: {
                        s32 x;
                        for (k = 0; k < 4; k++) {
                            if (val[k] >= 'A') val[k] -= 0x37;
                            if (val[k] >= 'a') val[k] -= 0x57;
                            if (val[k] >= '0') val[k] -= 0x30;
                        }
                        x = val[0] << 12;
                        x += val[1] << 8;
                        x += val[2] << 4;
                        x += val[3];
                        sysIni.mapNumber = x;
                        break;
                    }
                    case 46: {
                        s32 x;
                        for (k = 0; k < 4; k++) {
                            if (val[k] >= 'A') val[k] -= 0x37;
                            if (val[k] >= 'a') val[k] -= 0x57;
                            if (val[k] >= '0') val[k] -= 0x30;
                        }
                        x = val[0] << 12;
                        x += val[1] << 8;
                        x += val[2] << 4;
                        x += val[3];
                        sysIni.mapWarpNo = x;
                        break;
                    }
                    case 47:
                        if (val[0] == 'y') {
                            sysIni.viewerMip = 1;
                        } else {
                            sysIni.viewerMip = 0;
                        }
                        break;
                    case 48: case 49: case 50: case 51: case 52:
                    case 53: case 54: case 55: case 56: case 57:
                        ptyPartyData->bag[i - 48].id = atoi(val);
                        break;
                    case 58: case 59: case 60: case 61: case 62:
                    case 63: case 64: case 65: case 66: case 67:
                        ptyPartyData->bag[i - 58].count = atoi(val);
                        break;
                    case 68: case 69: case 70: case 71:
                    case 72: case 73: case 74: case 75:
                        sysIni.partyExt[i - 68] = atoi(val);
                        break;
                    case 76: {
                        s32 x;
                        for (k = 0; k < 4; k++) {
                            if (val[k] >= 'A') val[k] -= 0x37;
                            if (val[k] >= 'a') val[k] -= 0x57;
                            if (val[k] >= '0') val[k] -= 0x30;
                        }
                        x = val[0] << 12;
                        x += val[1] << 8;
                        x += val[2] << 4;
                        sysSaveData.party[0] = val[3];
                        sysIni.partyNum[0] = x;
                        sysIni.partyNum[0] += sysSaveData.party[0];
                        break;
                    }
                    case 77:
                        sysIni.party[0].exp = atoi(val);
                        break;
                    case 78: case 79: case 80: case 81: case 82: case 83:
                        sysIni.party[0].equip[i - 78] = atoi(val);
                        break;
                    case 84: case 85: case 86: case 87: case 88: case 89:
                        sysIni.party[0].slot[i - 84] = atoi(val);
                        break;
                    case 90: case 91: case 92: case 93: {
                        s32 x;
                        for (k = 0; k < 4; k++) {
                            if (val[k] >= 'A') val[k] -= 0x37;
                            else if (val[k] >= 'a') val[k] -= 0x57;
                            else if (val[k] >= '0') val[k] -= 0x30;
                        }
                        x = val[0] << 12;
                        x += val[1] << 8;
                        x += val[2] << 4;
                        x += val[3];
                        sysIni.monsNo[i - 90] = x;
                        break;
                    }
                    case 94: case 95: case 96: case 97:
                        sysIni.monsNum[i - 94] = atoi(val);
                        break;
                    case 98: case 99: case 100: case 101:
                        sysIni.monsLv[i - 98] = atoi(val);
                        break;
                    case 102:
                        if (val[0] == 'y') {
                            sysIni.startScnr = 1;
                        } else {
                            sysIni.startScnr = 0;
                        }
                        break;
                    case 103: {
                        s32 x;
                        for (k = 0; k < 4; k++) {
                            if (val[k] >= 'A') val[k] -= 0x37;
                            if (val[k] >= 'a') val[k] -= 0x57;
                            if (val[k] >= '0') val[k] -= 0x30;
                        }
                        x = val[0] << 12;
                        x += val[1] << 8;
                        x += val[2] << 4;
                        sysSaveData.party[1] = val[3];
                        sysIni.partyNum[1] = x;
                        sysIni.partyNum[1] += sysSaveData.party[1];
                        break;
                    }
                    case 104:
                        sysIni.party[1].exp = atoi(val);
                        break;
                    case 105: case 106: case 107: case 108: case 109: case 110:
                        sysIni.party[1].equip[i - 105] = atoi(val);
                        break;
                    case 111: case 112: case 113: case 114: case 115: case 116:
                        sysIni.party[1].slot[i - 111] = atoi(val);
                        break;
                    case 117: {
                        s32 x;
                        for (k = 0; k < 4; k++) {
                            if (val[k] >= 'A') val[k] -= 0x37;
                            if (val[k] >= 'a') val[k] -= 0x57;
                            if (val[k] >= '0') val[k] -= 0x30;
                        }
                        x = val[0] << 12;
                        x += val[1] << 8;
                        x += val[2] << 4;
                        sysSaveData.party[2] = val[3];
                        sysIni.partyNum[2] = x;
                        sysIni.partyNum[2] += sysSaveData.party[2];
                        break;
                    }
                    case 118:
                        sysIni.party[2].exp = atoi(val);
                        break;
                    case 119: case 120: case 121: case 122: case 123: case 124:
                        sysIni.party[2].equip[i - 119] = atoi(val);
                        break;
                    case 125: case 126: case 127: case 128: case 129: case 130:
                        sysIni.party[2].slot[i - 125] = atoi(val);
                        break;
                    case 131: {
                        s32 x;
                        for (k = 0; k < 4; k++) {
                            if (val[k] >= 'A') val[k] -= 0x37;
                            if (val[k] >= 'a') val[k] -= 0x57;
                            if (val[k] >= '0') val[k] -= 0x30;
                        }
                        x = val[0] << 12;
                        x += val[1] << 8;
                        x += val[2] << 4;
                        sysSaveData.party[3] = val[3];
                        sysIni.partyNum[3] = x;
                        sysIni.partyNum[3] += sysSaveData.party[3];
                        break;
                    }
                    case 132:
                        sysIni.party[3].exp = atoi(val);
                        break;
                    case 133: case 134: case 135: case 136: case 137: case 138:
                        sysIni.party[3].equip[i - 133] = atoi(val);
                        break;
                    case 139: case 140: case 141: case 142: case 143: case 144:
                        sysIni.party[3].slot[i - 139] = atoi(val);
                        break;
                    case 145:
                        sysIni.mainFlagNo = atoi(val);
                        break;
                    case 146: case 147: case 148: case 149:
                        sysIni.mapSeBank[i - 146] = atoi(val);
                        break;
                    }
                }
            }
        }
    }
    SysSetClearColor(0x80000000 | (sysIni.backColor[2] << 16) | (sysIni.backColor[1] << 8) | sysIni.backColor[0]);
    DbgSetClearColor(sysIni.backColor[0], sysIni.backColor[1], sysIni.backColor[2]);
    DbgLightAngleSet(0, sysIni.light0Angle);
    DbgLightAngleSet(1, sysIni.light1Angle);
}
