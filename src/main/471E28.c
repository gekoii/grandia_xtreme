/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[0x18];
    s64 pos;
} BitStream;

typedef struct {
    u64 id;
    s32 length;
    s32 scrambling;
    s64 pts;
    s64 dts;
    s32 dataPos;
    s32 dataLen;
    s32 headerPos;
} PesHeader;

void _sceMpegBsSkipBytes(BitStream *bs, s32 n);
extern char _sceMpegPackHeaderFieldFlagStr[];

s32 _sceMpegPesPacket(BitStream *bs, PesHeader *h) {
    u32 ptsDtsFlags;
    u32 escrFlag;
    u32 flags;
    u32 extFlag;
    u32 hdrLen;
    s32 start;
    u32 privFlag;
    u32 packFlag;
    u32 seqFlag;
    u32 pstdFlag;
    u32 ext2Flag;
    u32 n;
    u32 i;
    s32 len;
    s32 skip;
    s32 skip0;

    h->headerPos = bs->pos;
    _sceMpegBsGetBits(bs, 0x18);
    h->id = (u64)_sceMpegBsGetBits(bs, 8) << 32;
    h->length = _sceMpegBsGetBits(bs, 0x10);
    h->dts = -1;
    h->pts = -1;
    if (h->id != 0xBC00000000LL && h->id != 0xBE00000000LL && h->id != 0xBF00000000LL &&
        h->id != 0xF000000000LL && h->id != 0xF100000000LL && h->id != 0xFF00000000LL &&
        h->id != 0xF200000000LL && h->id != 0xF800000000LL) {
        _sceMpegBsGetBits(bs, 2);
        h->scrambling = _sceMpegBsGetBits(bs, 2);
        _sceMpegBsGetBits(bs, 4);
        ptsDtsFlags = _sceMpegBsGetBits(bs, 2);
        escrFlag = _sceMpegBsGetBits(bs, 1);
        flags = _sceMpegBsGetBits(bs, 4);
        extFlag = _sceMpegBsGetBits(bs, 1);
        hdrLen = _sceMpegBsGetBits(bs, 8);
        start = bs->pos;
        if (ptsDtsFlags & 2) {
            u32 a;
            u32 b;
            u32 c;
            _sceMpegBsGetBits(bs, 4);
            a = _sceMpegBsGetBits(bs, 3);
            _sceMpegBsMarkerBit(bs);
            b = _sceMpegBsGetBits(bs, 0xF);
            _sceMpegBsMarkerBit(bs);
            c = _sceMpegBsGetBits(bs, 0xF);
            ((void (*)(BitStream *))_sceMpegBsMarkerBit)(bs);
            h->pts = ((u64)((a >> 2) & 1) << 32) | (u32)((a << 30) | (b << 15) | c);
        }
        if (ptsDtsFlags == 3) {
            u32 a;
            u32 b;
            u32 c;
            _sceMpegBsGetBits(bs, 4);
            a = _sceMpegBsGetBits(bs, 3);
            _sceMpegBsMarkerBit(bs);
            b = _sceMpegBsGetBits(bs, 0xF);
            _sceMpegBsMarkerBit(bs);
            c = _sceMpegBsGetBits(bs, 0xF);
            ((void (*)(BitStream *))_sceMpegBsMarkerBit)(bs);
            h->dts = ((u64)((a >> 2) & 1) << 32) | (u32)((a << 30) | (b << 15) | c);
        }
        if (escrFlag == 1) {
            _sceMpegBsGetBits(bs, 0x30);
        }
        if (flags != 0) {
            _sceMpegBsGetBits(bs, _sceMpegPesFlagBits[flags]);
        }
        if (extFlag == 1) {
            privFlag = _sceMpegBsGetBits(bs, 1);
            packFlag = _sceMpegBsGetBits(bs, 1);
            seqFlag = _sceMpegBsGetBits(bs, 1);
            pstdFlag = _sceMpegBsGetBits(bs, 1);
            _sceMpegBsGetBits(bs, 3);
            ext2Flag = _sceMpegBsGetBits(bs, 1);
            if (privFlag == 1) {
                _sceMpegBsGetBits(bs, 0x30);
                _sceMpegBsGetBits(bs, 0x30);
                _sceMpegBsGetBits(bs, 0x20);
            }
            if (packFlag == 1) {
                _sceMpegError(_sceMpegPackHeaderFieldFlagStr);
                return 0;
            }
            if (seqFlag == 1) {
                _sceMpegBsGetBits(bs, 0x10);
            }
            if (pstdFlag == 1) {
                _sceMpegBsGetBits(bs, 0x10);
            }
            if (ext2Flag == 1) {
                _sceMpegBsMarkerBit(bs);
                n = _sceMpegBsGetBits(bs, 7);
                for (i = 0; i < n; i++) {
                    _sceMpegBsGetBits(bs, 8);
                }
            }
        }
        skip0 = hdrLen - (s32)((bs->pos - start) >> 3);
        if (skip0 != 0) {
            _sceMpegBsSkipBytes(bs, skip0);
        }
        len = h->length - hdrLen;
        skip = len - 3;
        h->dataLen = skip;
        h->dataPos = bs->pos;
        if (h->id == 0xBD00000000LL) {
            h->id |= _sceMpegBsGetBits(bs, 0x20);
            skip = len - 7;
        }
        if (skip != 0) {
            _sceMpegBsSkipBytes(bs, skip);
        }
    } else if (h->id == 0xBC00000000LL || h->id == 0xBF00000000LL || h->id == 0xF000000000LL ||
               h->id == 0xF100000000LL || h->id == 0xFF00000000LL || h->id == 0xF200000000LL ||
               h->id == 0xF800000000LL) {
        len = h->length;
        if (h->id == 0xBF00000000LL) {
            len -= 4;
            h->id |= _sceMpegBsGetBits(bs, 0x20);
        }
        if (len != 0) {
            _sceMpegBsSkipBytes(bs, len);
        }
    } else if (h->id == 0xBE00000000LL) {
        if (h->length != 0) {
            _sceMpegBsSkipBytes(bs, h->length);
        }
    }
    return 1;
}
