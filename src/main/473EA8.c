#include "common.h"
#include "protos/main.h"

/* Matches MSSG mpeg2decode's form_predictions (recon.c). Third-party library code: see README.md, "Third-party library code". */

typedef struct {
    u8 pad0[0x12C];
    s32 unk12C;
    u8 pad130[0x10];
} Slot;

typedef struct {
    Slot slots[2];
    s32 cur;
} SlotTable;

void _formPrediction(s32 src, s32 sfield, s32 dfield, s32 yoff, s32 h, s32 x, s32 y, s32 dx, s32 dy, s32 fieldFlag, s32 avg);
void _calcDMV(s32 *dmv, s32 *dmvector, s32 mvx, s32 mvy);

extern SlotTable _sceMpegMbSlots;
extern s32 _sceMpegFwdRefFrame;
extern s32 _sceMpegBwdRefFrame;
extern s32 _sceMpegFwdRefTop;
extern s32 _sceMpegFwdRefBottom;
extern s32 _sceMpegBwdRefTop;
extern s32 _sceMpegBwdRefBottom;
extern s32 Second_Field;
extern s32 picture_coding_type;
extern s32 picture_structure;
extern char _sceMpegInvalidMotionTypeAFmt[];
extern char _sceMpegInvalidMotionTypeBFmt[];
extern char _sceMpegInvalidMotionTypeCFmt[];

void _formPredictions(s32 bx, s32 by, s32 mbType, s32 motionType, s32 *pmv, s32 *mvfs, s32 *dmvector) {
    s32 dmv[2][2];
    s32 pred[2][2];
    s32 sel;
    s32 avg;
    s32 currentfield;
    s32 one;
    s32 two;

    one = 1;
    avg = 0;
    _sceMpegMbSlots.slots[_sceMpegMbSlots.cur].unk12C = 0;
    if ((mbType & 8) || picture_coding_type == 2) {
        if (picture_structure == 3) {
            if (motionType == 2 || !(mbType & 8)) {
                _formPrediction(_sceMpegFwdRefFrame, 0, 0, 0, 16, bx, by, pmv[0], pmv[1], 0, 0);
            } else if (motionType == one) {
                _formPrediction(_sceMpegFwdRefFrame, mvfs[0], 0, 0, 8, bx, by, pmv[0], pmv[1] >> 1, one, 0);
                _formPrediction(_sceMpegFwdRefFrame, mvfs[2], 1, 0, 8, bx, by, pmv[4], pmv[5] >> 1, one, 0);
            } else if (motionType == 3) {
                _calcDMV(&dmv[0][0], dmvector, pmv[0], pmv[1] >> 1);
                _formPrediction(_sceMpegFwdRefFrame, 0, 0, 0, 8, bx, by, pmv[0], pmv[1] >> 1, one, 0);
                _formPrediction(_sceMpegFwdRefFrame, 1, 0, 0, 8, bx, by, dmv[0][0], dmv[0][1], one, one);
                _formPrediction(_sceMpegFwdRefFrame, 1, 1, 0, 8, bx, by, pmv[0], pmv[1] >> 1, one, 0);
                _formPrediction(_sceMpegFwdRefFrame, 0, 1, 0, 8, bx, by, dmv[1][0], dmv[1][1], one, one);
            } else {
                _sceMpegErrorf(_sceMpegInvalidMotionTypeAFmt, motionType);
            }
        } else {
            two = 2;
            currentfield = (picture_structure == 2);
            sel = 0;
            pred[0][0] = _sceMpegFwdRefTop;
            pred[0][1] = _sceMpegFwdRefBottom;
            pred[1][0] = _sceMpegBwdRefTop;
            pred[1][1] = _sceMpegBwdRefBottom;
            if (picture_coding_type == two && Second_Field != 0) {
                sel = (currentfield != mvfs[0]);
            }
            if (motionType == 1 || !(mbType & 8)) {
                _formPrediction(pred[sel][mvfs[0]], 0, 0, 0, 16, bx, by, pmv[0], pmv[1], 0, 0);
            } else if (motionType == 2) {
                _formPrediction(pred[sel][mvfs[0]], 0, 0, 0, 8, bx, by, pmv[0], pmv[1], 0, 0);
                sel = 0;
                if (picture_coding_type == 2 && Second_Field != 0 && currentfield != mvfs[2]) {
                    sel = 1;
                }
                _formPrediction(pred[sel][mvfs[2]], 0, 0, 8, 8, bx, by, pmv[4], pmv[5], 0, 0);
            } else if (motionType == 3) {
                sel = 0;
                if (Second_Field != 0) {
                    sel = 1;
                }
                _calcDMV(&dmv[0][0], dmvector, pmv[0], pmv[1]);
                _formPrediction(pred[0][currentfield], 0, 0, 0, 16, bx, by, pmv[0], pmv[1], 0, 0);
                _formPrediction(pred[sel][!currentfield], 0, 0, 0, 16, bx, by, dmv[0][0], dmv[0][1], 0, 1);
            } else {
                _sceMpegErrorf(_sceMpegInvalidMotionTypeBFmt, motionType);
            }
        }
        avg = 1;
    }
    if (mbType & 4) {
        if (picture_structure == 3) {
            one = 1;
            if (motionType == 2) {
                _formPrediction(_sceMpegBwdRefFrame, 0, 0, 0, 16, bx, by, pmv[2], pmv[3], 0, avg);
            } else {
                _formPrediction(_sceMpegBwdRefFrame, mvfs[1], 0, 0, 8, bx, by, pmv[2], pmv[3] >> 1, one, avg);
                _formPrediction(_sceMpegBwdRefFrame, mvfs[3], 1, 0, 8, bx, by, pmv[6], pmv[7] >> 1, one, avg);
            }
        } else if (motionType == 1) {
            _formPrediction(mvfs[1] ? _sceMpegBwdRefBottom : _sceMpegBwdRefTop, 0, 0, 0, 16, bx, by, pmv[2], pmv[3], 0, avg);
        } else if (motionType == 2) {
            _formPrediction(mvfs[1] ? _sceMpegBwdRefBottom : _sceMpegBwdRefTop, 0, 0, 0, 8, bx, by, pmv[2], pmv[3], 0, avg);
            _formPrediction(mvfs[3] ? _sceMpegBwdRefBottom : _sceMpegBwdRefTop, 0, 0, 8, 8, bx, by, pmv[6], pmv[7], 0, avg);
        } else {
            _sceMpegErrorf(_sceMpegInvalidMotionTypeCFmt, motionType);
        }
    }
}
