#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

extern s32 movSkipFadeMode;

/* Word n of the fade ALPHA packet in image packet i (movPktFadeAlpha is a
 * word index into it). */
#define EL(i, n) (p->v[i] + movPktFadeAlpha)[n]

s32 MovVblankHandler(void) {
    VoTag *p;
    s32 idx;
    s32 c;
    s32 v;
    s32 s;

    movOddField = (s32)((*(volatile u64 *)0x12001000 >> 13) & 1);
    if (movDispActive == 0) {
        goto end; /* to the final return: an early return 0 changes the layout */
    }
    movVblankCount++;
    movPathBusy = sceGsSyncPath(1, 0);
    if (movPathBusy != 0) {
        return 0;
    }
    p = voBufGetTag(&movVoBuf);
    if (p == NULL) {
        movFrameMissCount++;
        movFrameStarve++;
        return 0;
    }
    idx = 0;
    if (movOddField == 0) {
        idx = 1;
    }
    if (movSkipRequest != 0) {
        if (movSkipFadeStep >= 8) {
            switch (movSkipFadeMode) {
            case 0:
                c = 0;
                break;
            case 1:
                c = 0x80;
                break;
            default:
                c = 0x80;
                break;
            case 2:
                EL(idx % 2, 12) = 0x80FFFFFF;
                c = 0;
                break;
            }
            if (idx == 1) {
                movFadeDone = idx;
            }
        } else {
            switch (movSkipFadeMode) {
            case 0:
                c = (7 - movSkipFadeStep) * 16;
                break;
            case 1:
                c = 0x80;
                break;
            default:
                c = 0x80;
                break;
            case 2:
                c = ((movSkipFadeStep + 1) * 255) / 8;
                v = c | 0x80000000;
                EL(idx % 2, 12) = v | ((c << 16) | (c << 8));
                c = (7 - movSkipFadeStep) * 16;
                break;
            }
        }
        EL(idx % 2, 1) = c;
        movSkipFadeStep++;
    } else {
        EL(idx, 1) = 0x80;
    }
    if (movOddField == 0 && p->status == 2) {
        if (movFadeDone == 0) {
            MovDispSetBuffer(&movDisp, 0);
            MovGifSendChain((u32)p->v[1]);
        }
        p->status = 1;
    } else if (movOddField != 0) {
        s = p->status;
        if (s == 1) {
            if (movFadeDone == 0) {
                MovDispSetBuffer(&movDisp, 1);
                MovGifSendChain((u32)p->v[0]);
            }
            p->status = 0;
            movFrameSent = s;
        }
    }
    movFrameStarve = 0;
end:
    return 0;
}
