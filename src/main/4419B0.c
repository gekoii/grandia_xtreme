#include "common.h"
#include "protos/main.h"
#include "main/n06.h"

s32 SndPlayAtPos(SndPlayPosReq *);

void SndSeUpdate(void) {
    s32 g;
    s32 i;
    s32 cnt;
    SndSeSlotShifted *s;
    SndSeSlotShifted *t;
    u8 a;
    s32 r;
    s32 io;

    if (sndSeActive == 0) {
        SndSeStopAll();
        return;
    }
    cnt = 0;
    for (g = 0; g < 4; g++) {
        for (i = 0; i < 64; i++) {
            s = &sndSeSlots[g].u.shifted[i];
            if (s->state == 0 && s->timer != 0) {
                s->timer--;
            }
        }
    }
    for (g = 0; g < 4; g++) {
        for (i = 0; i < 64; i++) {
            s = &sndSeSlots[g].u.shifted[i];
            if (s->state == 3) {
                sndSeWork.result = SndGetStatus(s->handle, &sndSeWork.status);
                if (sndSeWork.result == 0 && sndSeWork.status.stat == 0) {
                    if (s->flags & 0x10) {
                        s->state = 4;
                    } else {
                        s->state = 0;
                    }
                    s->timer = 0x3C;
                    SndSeVoiceStop(s->handle);
                }
            }
        }
    }
    for (g = 0; g < 4; g++) {
        for (i = 0; i < 64; i++) {
            io = i * 3 << 4;
            s = (SndSeSlotShifted *)((u8 *)sndSeSlots + io + g * 0xC10);
            a = s->state;
            if (a == 0 || a == 4) {
                continue;
            }
            if (a == 3 && !(s->flags & 8)) {
                continue;
            }
            if (s->wait != 0) {
                s->wait--;
                continue;
            }
            if (a == 1) {
                if (SndGetStatus(s->handle, &sndSeWork.status) != 0) {
                    continue;
                }
                if (sndSeWork.status.stat != 0) {
                    cnt++;
                    sndSeWork.prepare.handle = s->handle;
                    sndSeWork.prepare.index = s->index;
                    sndSeWork.prepare.unk8 = 0;
                    sndSeWork.prepare.unkC = 0;
                    if (SndPrepare(&sndSeWork.prepare) != 0) {
                        continue;
                    }
                }
            }
            t = (SndSeSlotShifted *)((u8 *)sndSeSlots + (io + g * 0xC10));
            t->state = 2;
            sndSeWork.result = SndGetStatus(sndSeWork.prepare.handle, &sndSeWork.status);
            if (sndSeWork.status.atr == 4) {
                SndSeClearHold(g, i);
                t->state = 0;
                SndStop(t->handle);
                continue;
            }
            if (t->flags & 8) {
                sndSeWork.playPos.handle = t->handle;
                sndSeWork.playPos.index = t->index;
                sndSeWork.playPos.pos = ((SndSeSlot *)((u8 *)sndSeSlots->u.s.slots + g * 0xC10 + io))->pos;
                r = SndPlayAtPos(&sndSeWork.playPos);
            } else {
                sndSeWork.play.handle = t->handle;
                sndSeWork.play.index = t->index;
                sndSeWork.play.dist = t->dist;
                sndSeWork.play.hPan = t->pan;
                sndSeWork.play.vPan = 0;
                r = SndPlay(&sndSeWork.play);
            }
            if (r == 0) {
                s->wait = 10;
                cnt++;
                s->state = 3;
                if (cnt >= 3) {
                    return;
                }
                continue;
            }
            switch (r) {
            case -1:
                SndSeStop(g, i);
                break;
            case -2:
                SndSeStop(g, i);
                break;
            case -3:
                break;
            }
        }
    }
}
