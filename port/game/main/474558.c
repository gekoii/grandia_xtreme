/* Port-only C for _formPrediction (0x00474558), which the decompilation still has in assembly. */
#include "common.h"
#include "sdk/libmpeg.h"

/* The view of a SceMpegPredArg that the prediction routines read */
typedef struct {
    /* 0x00 */ u32 dst;     /* in _sceMpegSprPred */
    /* 0x04 */ s32 x;       /* pel offset in the fetched block */
    /* 0x08 */ s32 rows0;   /* rows from the upper fetched block */
    /* 0x0C */ s32 rows1;   /* rows from the lower one */
    /* 0x10 */ s32 stride;  /* 16 or 8, doubled for field prediction */
    /* 0x14 */ u32 src0;    /* in the slot's predBuf */
    /* 0x18 */ u32 src1;
} PredArg;

typedef void (*PredFunc)(void *);

extern PredFunc _sceMpegPredYFuncs[8];
extern PredFunc _sceMpegPredCFuncs[8];

void _formPrediction(SceMpegPicture *src, s32 sfield, s32 dfield, s32 yoff, s32 h, s32 x, s32 y,
                     s32 dx, s32 dy, s32 fieldFlag, s32 avg);

/* Splits rows h starting at row yy of a size-pel macroblock into the rows
   in the upper and lower fetched block (half-pel vertical motion needs one
   more row) */
static void SplitRows(PredArg *a, s32 yy, s32 h, s32 size, s32 yh, s32 fieldFlag) {
    s32 t;

    if (yh) {
        if (yy + (h << fieldFlag) < size) {
            a->rows0 = h;
            a->rows1 = 0;
            return;
        }
        t = (size >> fieldFlag) - (yy >> fieldFlag) - 1;
    } else {
        if (yy + (h << fieldFlag) < size + 1) {
            a->rows0 = h;
            a->rows1 = 0;
            return;
        }
        t = (size >> fieldFlag) - (yy >> fieldFlag);
    }
    a->rows0 = t;
    a->rows1 = h - t;
}

/* MSSG mpeg2decode's form_prediction, queued: fills the current
   macroblock slot's next luma and chroma argument blocks, the addresses of
   the two macroblocks of src to fetch (the one holding the block and the
   one below) and the routines to run (picked by avg << 2 | x half-pel << 1
   | y half-pel), and counts the prediction. The reference fetch and the
   routines run later (_sceMpegFinishMacroblock). The 11 arguments are
   EABI's 8 registers and 3 stack words. */
void _formPrediction(SceMpegPicture *src, s32 sfield, s32 dfield, s32 yoff, s32 h, s32 x, s32 y,
                     s32 dx, s32 dy, s32 fieldFlag, s32 avg) {
    SceMpegMbSlot *slot = &_sceMpegMbSlots.slot[_sceMpegMbSlots.cur];
    s32 n = slot->predCount;
    PredArg *l = (PredArg *)&slot->predArgA[n];
    PredArg *c = (PredArg *)&slot->predArgB[n];
    u32 pred = _sceMpegSprPred;
    u32 lbase;
    u32 cbase;
    s32 xx, yy, bx, by, mb;
    s32 cxx, cyy, cbx, cby;
    s32 kl, kc;

    /* luma */
    xx = (dx >> 1) + x;
    if (fieldFlag) {
        yy = ((dy >> 1) << 1) + y + (yoff + sfield);
    } else {
        yy = (dy >> 1) + y + (yoff + sfield);
    }
    bx = xx >> 4;
    by = yy >> 4;
    mb = bx * src->mbHeight + by;
    l->x = xx - (bx << 4);
    l->dst = pred + ((dfield + yoff) << 5);
    yy -= by << 4;
    SplitRows(l, yy, h, 16, dy & 1, fieldFlag);
    kl = (avg << 2) | ((dx & 1) << 1) | (dy & 1);
    lbase = (u32)slot->predBuf + n * 0x600;
    l->stride = 16 << fieldFlag;
    l->src0 = lbase + (yy << 4);
    l->src1 = lbase + (yy << 4) + 0x300;

    /* chroma, at half the motion and position */
    dx /= 2;
    dy /= 2;
    cxx = (dx >> 1) + (x >> 1);
    if (fieldFlag) {
        cyy = ((dy >> 1) << 1) + (y >> 1) + ((yoff >> 1) + sfield);
    } else {
        cyy = (dy >> 1) + (y >> 1) + ((yoff >> 1) + sfield);
    }
    cbx = cxx >> 3;
    cby = cyy >> 3;
    c->x = cxx - (cbx << 3);
    c->dst = pred + ((dfield + (yoff >> 1)) << 4) + 0x200;
    cyy -= cby << 3;
    SplitRows(c, cyy, h >> 1, 8, dy & 1, fieldFlag);
    c->stride = 8 << fieldFlag;
    kc = (avg << 2) | ((dx & 1) << 1) | (dy & 1);
    cbase = lbase + ((cbx - bx) * 2 + (cby - by)) * 0x180;
    c->src0 = cbase + (cyy << 3) + 0x100;
    c->src1 = cbase + (cyy << 3) + 0x400;

    slot->predSrcA[n] = src->addr + mb * 0x180;
    slot->predFuncA[n] = _sceMpegPredYFuncs[kl];
    slot->predSrcB[n] = src->addr + (mb + src->mbHeight) * 0x180;
    slot->predFuncB[n] = _sceMpegPredCFuncs[kc];
    slot->predCount = slot->predCount + 1;
}
