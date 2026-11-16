#include "common.h"
#include "pad.h"
#include "snd.h"
#include "protos/main.h"

extern s32 dbgSndBgmNo;
extern s32 dbgSndBgmLine;
extern s32 dbgSndBgmCursor;
extern u8 dbgSndBgmFade;
extern f32 dbgSndBgmVolume;
extern u8 dbgSndBgmStrReqNo[];
extern u8 dbgSndBgmStrReqFade[];
extern u8 dbgSndBgmStrReqVolume[];
extern u8 dbgSndBgmStrProcess[];
extern u8 dbgSndBgmStrReqStop[];
extern u8 dbgSndBgmStrVolume[];
extern u8 dbgSndBgmStrFade[];
extern u8 dbgSndBgmStrVmsndIndex[];
extern u8 dbgSndBgmStrIndex[];
extern u8 dbgSndBgmStrInitdata[];
extern u8 dbgSndBgmStrInitsize[];
extern u8 dbgSndBgmStrVmsndOpen[];
extern u8 dbgSndBgmStrOpenObjnum[];
extern u8 dbgSndBgmStrOpenIndexnum[];
extern u8 dbgSndBgmStrHandle[];
extern u8 dbgSndBgmStrSeclen[];
extern u8 dbgSndBgmStrVmsndStat[];
extern u8 dbgSndBgmStrStat[];
extern u8 dbgSndBgmStrAtr[];
extern u8 dbgSndBgmStrCate[];
extern u8 dbgSndBgmStrError[];
extern u8 dbgSndBgmStrNowvol[];
extern u8 dbgSndBgmStrDisvol[];
extern u8 dbgSndBgmStrKtime[];
extern u8 dbgSndBgmStrVmsndOn1[];
extern u8 dbgSndBgmStrOn1Objnum[];
extern u8 dbgSndBgmStrOn1Indexnum[];
extern u8 dbgSndBgmStrDist[];
extern u8 dbgSndBgmStrHpan[];
extern u8 dbgSndBgmStrVpan[];

void DbgSoundBgmPage(void) {
    s32 t;

    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        t = DbgMenuLineSelect(dbgSndBgmLine, 2);
        dbgSndBgmLine = t;
        switch (t) {
        case 0:
            dbgSndBgmCursor = t;
            if (!(dbgPad->held & 1)) {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgSndBgmNo -= 2;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgSndBgmNo += 2;
                }
            } else {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgSndBgmNo -= 20;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgSndBgmNo += 20;
                }
            }
            if (dbgSndBgmNo < 0) {
                dbgSndBgmNo = 0;
            }
            if (dbgPad->repeatFast & 0x20) {
                if (!(dbgPad->held & 1)) {
                    SndBgmPlay(dbgSndBgmNo, dbgSndBgmFade);
                } else {
                    SndStreamPlay(dbgSndBgmNo, dbgSndBgmFade, 0xAA);
                }
            }
            break;
        case 1:
            dbgSndBgmCursor = t;
            if (dbgPad->repeatFast & 0x8000) {
                dbgSndBgmFade--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgSndBgmFade++;
            }
            if (dbgPad->repeatFast & 0x20) {
                if (!(dbgPad->held & 1)) {
                    SndBgmPlay(dbgSndBgmNo, dbgSndBgmFade);
                } else {
                    SndStreamPlay(dbgSndBgmNo, dbgSndBgmFade, 0xAA);
                }
            }
            break;
        case 2:
            dbgSndBgmCursor = t;
            if (dbgPad->repeatFast & 0x8000) {
                dbgSndBgmVolume -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgSndBgmVolume += 1.0f;
            }
            if (dbgPad->repeatFast & 0x20) {
                SndBgmPlay(dbgSndBgmNo, dbgSndBgmFade);
                SndBgmFadeVolume(0, 0x5A, dbgSndBgmVolume);
                SndBgmFadeVolume(1, 0x5A, dbgSndBgmVolume);
            }
            break;
        }
    }
    DbgMenuCursorDraw(2, dbgSndBgmCursor + 4);
    DbgPrintf(3, 4, dbgSndBgmStrReqNo, dbgSndBgmNo);
    DbgPrintf(3, 5, dbgSndBgmStrReqFade, dbgSndBgmFade);
    DbgPrintf(3, 6, dbgSndBgmStrReqVolume, dbgSndBgmVolume);
    DbgPrintf(3, 7, dbgSndBgmStrProcess, sndBgmChannels[0].state, sndBgmChannels[1].state);
    DbgPrintf(3, 8, dbgSndBgmStrReqStop, sndBgmChannels[0].reqStop, sndBgmChannels[1].reqStop);
    DbgPrintf(3, 9, dbgSndBgmStrVolume, sndBgmChannels[0].volume, sndBgmChannels[1].volume);
    DbgPrintf(3, 10, dbgSndBgmStrFade, sndBgmChannels[0].fade, sndBgmChannels[1].fade);
    DbgPrintf(2, 11, dbgSndBgmStrVmsndIndex);
    DbgPrintf(3, 12, dbgSndBgmStrIndex, sndBgmChannels[0].drvIndex);
    DbgPrintf(3, 13, dbgSndBgmStrInitdata, sndBgmChannels[0].drvInitData);
    DbgPrintf(3, 14, dbgSndBgmStrInitsize, sndBgmChannels[0].drvInitSize);
    DbgPrintf(2, 15, dbgSndBgmStrVmsndOpen);
    DbgPrintf(3, 16, dbgSndBgmStrOpenObjnum, sndBgmChannels[0].prepare.handle, sndBgmChannels[1].prepare.handle);
    DbgPrintf(3, 17, dbgSndBgmStrOpenIndexnum, sndBgmChannels[0].prepare.index, sndBgmChannels[1].prepare.index);
    DbgPrintf(3, 18, dbgSndBgmStrHandle, sndBgmChannels[0].prepare.unk8, sndBgmChannels[1].prepare.unk8);
    DbgPrintf(3, 19, dbgSndBgmStrSeclen, sndBgmChannels[0].prepare.unkC, sndBgmChannels[1].prepare.unkC);
    DbgPrintf(0x21, 5, dbgSndBgmStrVmsndStat);
    DbgPrintf(0x22, 6, dbgSndBgmStrStat, sndBgmChannels[0].status.stat);
    DbgPrintf(0x22, 7, dbgSndBgmStrAtr, sndBgmChannels[0].status.atr);
    DbgPrintf(0x22, 8, dbgSndBgmStrCate, sndBgmChannels[0].status.cate);
    DbgPrintf(0x22, 9, dbgSndBgmStrError, sndBgmChannels[0].status.error);
    DbgPrintf(0x22, 10, dbgSndBgmStrNowvol, sndBgmChannels[0].status.nowVol);
    DbgPrintf(0x22, 11, dbgSndBgmStrDisvol, sndBgmChannels[0].status.disVol);
    DbgPrintf(0x22, 12, dbgSndBgmStrKtime, sndBgmChannels[0].status.kTime);
    DbgPrintf(0x22, 13, dbgSndBgmStrKtime, sndBgmChannels[0].status.kTime);
    DbgPrintf(0x21, 14, dbgSndBgmStrVmsndOn1);
    DbgPrintf(0x22, 15, dbgSndBgmStrOn1Objnum, sndBgmChannels[0].play.handle);
    DbgPrintf(0x22, 16, dbgSndBgmStrOn1Indexnum, sndBgmChannels[0].play.index);
    DbgPrintf(0x22, 17, dbgSndBgmStrDist, sndBgmChannels[0].play.dist);
    DbgPrintf(0x22, 18, dbgSndBgmStrHpan, sndBgmChannels[0].play.hPan);
    DbgPrintf(0x22, 19, dbgSndBgmStrVpan, sndBgmChannels[0].play.vPan);
}
