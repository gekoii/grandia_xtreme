#include "common.h"
#include "protos/main.h"
#include "main/n10.h"

extern SndHandle *sndSeVoiceHandle[];

s32 SndPlay(SndPlayReq *req) {
    SndHandle *p;
    SndIndexEntry *e;
    s16 ch;
    s32 s;
    s32 n;
    s32 m;
    s32 t;
    s32 t2;
    s8 flags;
    s32 a;
    s32 b;
    s8 state;

    p = SndFindHandle(req->handle);
    if (p == NULL) {
        return -7;
    }
    e = &sndIndexTable[req->index];
    state = p->state;
    flags = e->bank;
    ch = flags & 0xF;
    switch (state) {
    case 3:
        switch (p->type) {
        case 5:
            if (sndIopStatus->bankState[ch] == 0) {
                return -2;
            }
            switch (flags & 0xE0) {
            case 0x40:
                req->dist = req->dist >> 1;
                break;
            case 0x80:
                req->dist = (req->dist * 3) >> 3;
                break;
            case 0x20:
                req->dist = 0;
                break;
            case 0xC0:
                req->dist >>= 1;
            case 0:
                if (req->dist < 3) {
                    req->hPan = 0;
                }
                break;
            }
            if (req->dist >= 0x100) {
                if (SndHandleStop(p) < 0) {
                    return -2;
                }
                return -8;
            }
            t = e->u.s.volume;
            SndCalcVolPan(t & 0x7FFF, p->curVol, req->dist, req->hPan, &a, &b);
            s = p->dist - req->dist;
            s = p->unk38 * s + 0x10000;
            if (s != p->unk34 || p->volL != a || p->volR != b) {
                SndIopCmdPush(0x238, (p->voice << 24) | s, (ch << 16) | p->unk7, (a << 16) | b);
                p->unk34 = s;
                p->volL = a;
                sndSeVoiceVol[p->voice] = a + b;
                p->volR = b;
            }
            p->dist = req->dist;
            p->pan = req->hPan;
            break;
        case 10:
            break;
        default:
            return -4;
        }
        break;
    case 0 ... 2:
        p->id = req->handle;
        switch (e->type & 0x7F) {
        case 5:
            if (sndIopStatus->bankState[ch] != 2) {
                return -2;
            }
            switch (flags & 0xE0) {
            case 0x20:
                req->dist = 0;
                break;
            case 0x40:
                req->dist = req->dist >> 1;
                break;
            case 0x80:
                req->dist = (req->dist * 3) >> 3;
                break;
            case 0xC0:
                req->dist >>= 1;
            default:
                if (req->dist < 3) {
                    req->hPan = 0;
                }
                break;
            }
            if (req->dist >= 0x100) {
                return -8;
            }
            if (e->type & 0x80) {
                if (p->state == 0) {
                    p->fadeSpeed = 0;
                    p->curVol = 10000;
                    p->targetVol = 10000;
                    p->unk38 = 0x100;
                }
                t2 = e->u.s.volume;
                m = t2 & 0x7FFF;
                SndCalcVolPan(m, p->curVol, req->dist, req->hPan, &a, &b);
                n = SndAllocSeVoice(a + b);
                if (n < 0) {
                    return -7;
                }
                p->type = e->type & 0x7F;
                p->unk6 = e->unk1;
                p->unk7 = e->slot;
                p->baseVol = m;
                p->chan = ch;
                p->dist = req->dist;
                p->pan = req->hPan;
                p->index = req->index;
                p->unk34 = 0x10000;
                if (SndIopCmdPush(0x210, n, (ch << 16) | p->unk7, (a << 16) | b) != 0) {
                    p->volL = a;
                    p->volR = b;
                    p->voice = n;
                    p->timer = 0;
                    sndSeVoiceVol[n] = a + b;
                    sndSeVoiceHandle[n] = p;
                    sndSeVoiceTimer[n] = p->duration = e->u.s.duration;
                    sndSeVoiceState[n] = 1;
                    p->state = 3;
                    break;
                }
                return -11;
            }
            return -1;
        case 4:
            if (state > 0) {
                p->id = req->handle;
                p->unk7 = 1;
                p->timer = 0;
                break;
            }
            return -2;
        case 10:
            break;
        default:
            return -1;
        }
        break;
    }
    p->index = req->index;
    return 0;
}
