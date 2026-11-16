/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"





#define REG(a) (*(volatile u32 *)(a))

void _sceMpegCopyRaw8(SceMpegPicture *img) {
    SceMpegSys *st;
    u32 src;
    u32 dst;
    u32 d;
    s32 size;
    s32 qwc;
    s32 step;
    s32 passes;
    s32 i;
    s32 j;

    st = _sceMpegCurMp->sys;
    src = img->addr & 0x0FFFFFFF;
    dst = st->imageAddr & 0x0FFFFFFF;
    if (picture_structure == 3 || st->imageHeight == 0) {
        size = img->mbHeight * 0x180;
        qwc = size >> 4;
        if (st->imageHeight != 0) {
            step = (st->imageHeight >> 4) * 0x180;
        } else {
            step = size;
        }
        passes = 1;
    } else {
        step = (st->imageHeight >> 4) * 0xC0;
        size = (img->mbHeight >> 1) * 0x180;
        passes = 2;
        qwc = size >> 4;
    }
    for (j = 0; j < passes; j++) {
        d = dst;
        for (i = 0; i < img->mbWidth; i++) {
            REG(0x1000D480) = 0;
            REG(0x1000D410) = src;
            REG(0x1000D420) = qwc;
            REG(0x1000D400) = 0x101;
            while (REG(0x1000D400) & 0x100) {
            }
            REG(0x1000D080) = 0;
            REG(0x1000D010) = d;
            REG(0x1000D020) = qwc;
            REG(0x1000D000) = 0x100;
            while (REG(0x1000D000) & 0x100) {
            }
            while (REG(0x1000D020) != 0) {
            }
            d += step;
            src += size;
        }
        dst += st->imageSize * 0xC0;
    }
}
