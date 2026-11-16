#include "common.h"
#include "protos/main.h"
/* Follows MSSG mpeg2decode's motion_compensation (getpic.c), adapted for the PS2's IPU. Third-party library code: see README.md, "Third-party library code". */

typedef struct {
    s32 f0;
    s32 f4;
    s32 a8[4];
    s32 a18[4];
    u8 pad28[0x128 - 0x28];
    u8 *f128;
    s32 count;
    s32 f130;
    s32 f134;
    s32 f138;
    s32 f13C;
} Slot;

typedef struct {
    Slot e[2];
    s32 cur;
} SlotTable;

typedef struct {
    u8 *f0;
    u8 pad4[0xC];
    s32 f10;
} Frame;


extern s32 _sceMpegRefDmaTags;
extern s32 Fault_Flag;
extern s32 mb_width;
extern s32 picture_structure;
extern Frame *_sceMpegCurFrame;
extern Frame *_sceMpegCurTop;
extern Frame *_sceMpegCurBottom;
extern SlotTable _sceMpegMbSlots;
extern u8 _sceMpegInvalidMotionTypeFmt[];

s32 _motionCompensation(s32 MBA, s32 dct, s32 mbType, s32 motionType, s32 *pmv, s32 *mvfs, s32 *dmvector) {
    s32 x;
    s32 y;
    s32 bx;
    s32 by;
    s32 id;
    s32 intra;
    s32 i;
    s32 n;
    u64 *p;
    Frame *f;

    x = MBA % mb_width;
    y = MBA / mb_width;
    bx = x * 16;
    by = y * 16;
    intra = mbType & 1;
    if (intra) {
        while ((*(volatile u32 *)0x1000D400 >> 8) & 1) {
        }
        _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f138 = 0;
    } else {
        if ((u32)(motionType - 1) >= 3) {
            _sceMpegErrorf((s32)_sceMpegInvalidMotionTypeFmt, motionType);
            Fault_Flag = 1;
            return 0;
        }
        _formPredictions(bx, by, mbType, motionType, pmv, mvfs, dmvector);
        while ((*(volatile u32 *)0x1000D400 >> 8) & 1) {
        }
        n = _sceMpegMbSlots.e[_sceMpegMbSlots.cur].count;
        p = (u64 *)((_sceMpegRefDmaTags & 0x0FFFFFFF) | 0x20000000);
        for (i = 0; i < n; i++) {
            id = (i == n - 1) ? 0 : 3;
            p[0] = ((u64)(_sceMpegMbSlots.e[_sceMpegMbSlots.cur].a8[i] & 0x0FFFFFFF) << 32) | 0x30000030;
            p[2] = ((u64)(_sceMpegMbSlots.e[_sceMpegMbSlots.cur].a18[i] & 0x0FFFFFFF) << 32) | ((u64)id << 28) | 0x30;
            p += 4;
        }
        __asm__ volatile("sync");
        *(volatile u32 *)0x1000D480 = _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f0;
        *(volatile u32 *)0x1000D430 = _sceMpegRefDmaTags;
        *(volatile u32 *)0x1000D420 = 0;
        *(volatile u32 *)0x1000D400 = 0x105;
        _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f138 = 1;
    }
    if (dct == 1 && (mbType & 2)) {
        _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f134 = dct;
    } else {
        _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f134 = 0;
    }
    _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f130 = intra;
    if (picture_structure == 3) {
        _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f128 = _sceMpegCurFrame->f0 + (x * _sceMpegCurFrame->f10 + y) * 0x180;
    } else {
        f = (picture_structure == 2) ? _sceMpegCurBottom : _sceMpegCurTop;
        _sceMpegMbSlots.e[_sceMpegMbSlots.cur].f128 = f->f0 + (x * f->f10 + y) * 0x180;
    }
    return 1;
}
