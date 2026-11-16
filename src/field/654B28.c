/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

typedef struct {
    u16 unk0;
    u16 pad2;
    u16 unk4;
} PadIn;


extern f32 VuVecNormalizeLen(Vec4 *, Vec4 *);
extern s8 wmapDirHold[4];
extern s8 wmapDir;
extern u16 wmapStickRepeat;
extern u16 wmapPadTrig;

s32 WmapReadDir(PadIn *in) {
    Vec4 v;
    f32 len;
    u16 cur;
    u16 trg;

    wmapDir = -1;
    v.x = 0x80 - padPort0[0].leftX;
    v.y = padPort0[0].leftY - 0x80;
    v.z = 0.0f;
    len = VuVecNormalizeLen(&v, &v);
    cur = in->unk0 & 0xF000;
    trg = in->unk4 & 0xF000;
    if (cur == 0x9000 && (trg & 0x9000)) {
        wmapDir = 1;
        wmapDirHold[1] = 7;
        wmapDirHold[0] = 7;
    } else {
        goto tests;
    /* goto-built layout: the arms come first and the tests below jump back to them, as laid out in the original */
    b2:
        wmapDirHold[2] = 7;
        wmapDir = 3;
        wmapDirHold[1] = 7;
        goto end;
    b3:
        wmapDirHold[3] = 7;
        wmapDir = 5;
        wmapDirHold[2] = 7;
        goto end;
    b4:
        wmapDirHold[0] = 7;
        wmapDirHold[3] = 7;
        wmapDir = 7;
        goto end;
    b5:
        wmapDir = 0;
        wmapDirHold[0]++;
        goto end;
    b6:
        wmapDirHold[1]++;
        wmapDir = 2;
        goto end;
    b7:
        wmapDirHold[2]++;
        wmapDir = 4;
        goto end;
    b8:
        wmapDir = 6;
        wmapDirHold[3]++;
        goto end;
        /* do-while(0): keeps the register choice and block layout of the goto-targeted tests */
        do {
    tests:
        if (cur == 0xC000 && (trg & 0xC000)) goto b2;
        if (cur == 0x6000 && (trg & 0x6000)) goto b3;
        if (cur == 0x3000 && (trg & 0x3000)) goto b4;
        if (cur == 0x1000 && wmapDirHold[0] == 6) goto b5;
        if (cur == 0x8000 && wmapDirHold[1] == 6) goto b6;
        if (cur == 0x4000 && wmapDirHold[2] == 6) goto b7;
        if (cur == 0x2000 && wmapDirHold[3] == 6) goto b8;
        if (!(len < 70.0f)) {
            if (wmapStickRepeat != 0) {
                wmapStickRepeat--;
            } else {
                wmapStickRepeat = 5;
            }
        }
        } while (0);
    }
end:
    if (cur & 0x1000) {
        if (wmapDirHold[0] < 6) {
            wmapDirHold[0]++;
        }
    }
    if (cur & 0x8000) {
        if (wmapDirHold[1] < 6) {
            wmapDirHold[1]++;
        }
    }
    if (cur & 0x4000) {
        if (wmapDirHold[2] < 6) {
            wmapDirHold[2]++;
        }
    }
    if (cur & 0x2000) {
        if (wmapDirHold[3] < 6) {
            wmapDirHold[3]++;
        }
    }
    if (!(cur & 0x1000)) {
        wmapDirHold[0] = 0;
    }
    if (!(cur & 0x8000)) {
        wmapDirHold[1] = 0;
    }
    if (!(cur & 0x4000)) {
        wmapDirHold[2] = 0;
    }
    if (!(cur & 0x2000)) {
        wmapDirHold[3] = 0;
    }
    wmapPadTrig = trg;
    return wmapDir;
}
