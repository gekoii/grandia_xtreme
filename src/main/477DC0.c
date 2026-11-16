/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"





s32 _updatePictureBuffers(s32 second)
{
    SceMpegSys *m;
    SceMpegPicture *p;
    SceMpegPicture *tmp;
    SceMpegPicture *sel;
    s32 ret;
    s32 lim;
    s32 ps;

    m = _sceMpegCurMp->sys;
    p = NULL;
    ret = 0;
    ps = picture_structure;
    lim = (ps == 3) ? 2 : 4;
    if (picture_coding_type == 3) {
        _sceMpegCurFrame = _sceMpegAuxFrame;
        _sceMpegCurTop = _sceMpegAuxTop;
        _sceMpegCurBottom = _sceMpegAuxBottom;
        if (m->countI + m->countP >= lim) {
            m->unkE8 = 0;
            broken_link = 0;
            closed_gop = 0;
        }
        if ((m->unkE8 != 0 || broken_link != 0) && closed_gop == 0) {
            _sceMpegFwdRefFrame->ref = 0;
            _sceMpegFwdRefTop->ref = 0;
            _sceMpegFwdRefBottom->ref = 0;
        }
        m->unkE8 = 0;
        broken_link = 0;
        if (picture_structure == 3) {
            if (_sceMpegFwdRefFrame->ref == 1 || closed_gop != 0) {
                if (_sceMpegBwdRefFrame->ref == 1) {
                    ret = 1;
                }
            }
        } else {
            if ((_sceMpegFwdRefTop->ref == 1 && _sceMpegFwdRefBottom->ref == 1) || closed_gop != 0) {
                if (_sceMpegBwdRefTop->ref == 1) {
                    if (_sceMpegBwdRefBottom->ref == 1) {
                        ret = 1;
                    }
                }
            }
        }
    } else {
        if (!second) {
            tmp = _sceMpegFwdRefFrame;
            _sceMpegFwdRefFrame = _sceMpegBwdRefFrame;
            _sceMpegBwdRefFrame = tmp;
            tmp = _sceMpegFwdRefTop;
            _sceMpegFwdRefTop = _sceMpegBwdRefTop;
            _sceMpegBwdRefTop = tmp;
            tmp = _sceMpegFwdRefBottom;
            _sceMpegFwdRefBottom = _sceMpegBwdRefBottom;
            _sceMpegBwdRefBottom = tmp;
        }
        _sceMpegCurFrame = _sceMpegBwdRefFrame;
        _sceMpegCurTop = _sceMpegBwdRefTop;
        _sceMpegCurBottom = _sceMpegBwdRefBottom;
        if (picture_structure == 3) {
            if (picture_coding_type != 2 || _sceMpegFwdRefFrame->ref == 1) {
                ret = 1;
            }
        } else {
            sel = (picture_structure == 1) ? _sceMpegBwdRefBottom : _sceMpegBwdRefTop;
            if (picture_coding_type != 2 || (second && sel->ref == 1)
                || (_sceMpegFwdRefTop->ref == 1 && _sceMpegFwdRefBottom->ref == 1)) {
                ret = 1;
            }
        }
    }
    switch (picture_structure) {
    case 3:
        p = _sceMpegCurFrame;
        break;
    case 1:
        p = _sceMpegCurTop;
        break;
    case 2:
        p = _sceMpegCurBottom;
        break;
    }
    p->pts = _sceMpegPicPts;
    p->dts = _sceMpegPicDts;
    p->pictureCodingType = picture_coding_type;
    p->pictureStructure = picture_structure;
    p->progressiveSequence = progressive_sequence;
    p->progressiveFrame = progressive_frame;
    p->topFieldFirst = top_field_first;
    p->repeatFirstField = repeat_first_field;
    p->ref = 0;
    p->frameCenterOffset[0] = frame_center_horizontal_offset[0];
    p->frameCenterOffset[1] = frame_center_horizontal_offset[1];
    p->frameCenterOffset[2] = frame_center_horizontal_offset[2];
    p->frameCenterOffset[3] = frame_center_vertical_offset[0];
    p->frameCenterOffset[4] = frame_center_vertical_offset[1];
    p->frameCenterOffset[5] = frame_center_vertical_offset[2];
    p->displayHorizontalSize = display_horizontal_size;
    p->displayVerticalSize = display_vertical_size;
    return ret;
}
