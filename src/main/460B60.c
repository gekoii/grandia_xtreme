#include "common.h"
#include "main/n10.h"
#include "protos/main.h"


extern s32 sndSpuBankCount;
extern s32 sndSpuStreamBufCount;
extern s32 sndIniTrackCount;
extern char sndIniFmtDec[];
extern char sndIniFmtHex[];
extern char sndIniFmtHexPair[];
extern char sndStrmIndexName[];
extern char sndStrmDataName[];

s32 SndFloatToInt(f32);
void strcpy(char *, char *);

s32 SndDriverInit(char **arg0) {
    s32 info[2];
    char line[0x80];
    s32 v1;
    s32 v2;
    char *p;
    s32 i;
    s32 a;
    s32 b;
    s32 c;
    SndSoftVoice *vp;

    a = 10000;
    c = 10000;
    b = 9900;
    SndIopCmdPush(0x200, 0, 0, 0);
    sndIndexTable = (SndIndexEntry *)arg0[0];
    SndIopPoll();
    for (i = 0; i < 256; i++) {
        sndVolumeCurve[i] = SndFloatToInt(powf(0.98f, SndIntToFloat(i)) * 9940.0f) + 60;
        a = a * 9800 / 10000;
        sndFadeInStep[i] = c;
        c = 10000 / (i + 1);
        sndFadeOutRate[i] = 10000 - b;
        b = a * 9850 / 10000;
    }
    sndVolumeCurve[255] = 0;
    do {
        SndBusyWait();
        SndIopPoll();
    } while (sndIopStatus->ready == 0);
    p = SndIniReadLine(line, arg0[1]);
    sscanf(line, sndIniFmtDec, &sndSpuBankCount);
    p = SndIniReadLine(line, p);
    sscanf(line, sndIniFmtDec, &sndSpuStreamBufCount);
    p = SndIniReadLine(line, p);
    sscanf(line, sndIniFmtDec, &sndIniTrackCount);
    for (i = 0; i < sndSpuBankCount; i++) {
        p = SndIniReadLine(line, p);
        sscanf(line + 2, sndIniFmtHex, &v1);
        SndIopCmdPush(0x202, i, v1, 0);
    }
    SndIopPoll();
    SndBusyWait();
    for (i = 0; i < sndSpuStreamBufCount; i++) {
        p = SndIniReadLine(line, p);
        sscanf(line + 2, sndIniFmtHexPair, &v1, &v2);
        SndIopCmdPush(0x204, i, v1, v2);
    }
    p = SndIniReadLine(line, p);
    strcpy(sndStrmIndexName, line);
    p = SndIniReadLine(line, p);
    strcpy(sndStrmDataName, line);
    if (SndFsLoadFile(sndStrmIndexName, (s32)sndStrmIndex) < 0 || SndFsFind(sndStrmDataName, info) < 0) {
        return -1;
    }
    sndListenerPos.y = 0.0f;
    sndListenerPos.x = 0.0f;
    sndListenerPos.z = 0.0f;
    sndStrmDataLsn = info[0];
    sndListenerDir.x = 0.0f;
    sndListenerDir.y = 0.0f;
    sndListenerDir.z = 1.0f;
    vp = sndSoftVoices;
    do {
        vp->state = 0;
        vp->pos = (s32)sndPcmBuf;
        vp->unkC = 0;
        vp->env = 0;
        vp->unk2C = 0;
        vp->loopStart = (s32)sndPcmBuf;
        vp->end = (s32)&sndPcmBuf[0x40];
        vp->handle = sndHandles;
        vp++;
    } while ((s32)vp < (s32)(sndSoftVoices + 8));
    SndPcmMonitorOff();
    sndInitDone = 1;
    return 0;
}
