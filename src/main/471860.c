/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

/* The 32-bit callback fields need their own alias set (not int), so the
   header loads can be scheduled across the stores into cbd. */
typedef enum { CB_0 } CbEnum;

typedef struct {
    CbEnum type;
    CbEnum x4;
    CbEnum x8;
    CbEnum xC;
    u64 x10;
    u64 x18;
} CbData;

typedef struct {
    u64 key;
    u64 mask;
    s32 (*func)(void *, CbData *, s32);
    s32 data;
} Ent;

typedef struct {
    u8 pad[0x44];
    Ent *ents;
    s32 count;
} Sub;

typedef struct {
    u8 pad[0x40];
    Sub *sub;
} Mpeg;

typedef struct {
    u8 pad[0x18];
    u64 pos;
    u8 pad20[0x10];
} Bs;

typedef struct {
    u8 pad[0x18];
    u64 code;
    u8 pad20[0x8];
    u64 x28;
    u64 x30;
    s32 x38;
    s32 x3C;
    s32 x40;
    u8 pad44[0xC];
} Hdr;

void _sceMpegBsInit(Bs *, u32, s32, s32);
s32 _sceMpegPackHeader(Bs *, Hdr *);
s32 _sceMpegPesPacket(Bs *, u64 *);

s32 sceMpegDemuxPssRing(Mpeg *mp, u32 a1, s32 size, s32 a3, s32 a4) {
    Bs bsbuf;
    Hdr hdr;
    CbData cbd;
    Bs *bs;
    Hdr *hp;
    Sub *s;
    s32 (*defFunc)(void *, CbData *, s32) = NULL;
    s32 defData = 0;
    Ent *ents;
    s32 result;
    s32 ret;
    s32 i;

    ret = 1;
    hp = &hdr;
    i = 0;
    s = mp->sub;
    ents = s->ents;
    _sceMpegBsInit(&bsbuf, a1, a3, a4);
    bs = &bsbuf;
    result = 0;
    while (defFunc == NULL && i < s->count) {
        if (ents[i].key == 0xBDFF000000ULL) {
            defFunc = ents[i].func;
            defData = ents[i].data;
        }
        i++;
    }
    for (;;) {
        if (_sceMpegBsShowBits(bs, 32) == 0x1BA) {
            _sceMpegPackHeader(bs, hp);
        }
        while (_sceMpegBsShowBits(bs, 24) == 1 && _sceMpegBsShowBits(bs, 32) != 0x1BA &&
               _sceMpegBsShowBits(bs, 32) != 0x1B9 && bs->pos < size * 8 && ret != 0) {
            _sceMpegPesPacket(bs, &hp->code);
            if (size * 8 < bs->pos) {
                continue;
            }
            for (i = 0; i < s->count; i++) {
                if (ents[i].key == (hp->code & ents[i].mask)) {
                    cbd.type = 6;
                    cbd.x4 = _sceMpegBsGetPtr(bs, hp->x40);
                    cbd.x8 = _sceMpegBsGetPtr(bs, hp->x38);
                    cbd.xC = hp->x3C;
                    cbd.x10 = hp->x28;
                    cbd.x18 = hp->x30;
                    ret = ents[i].func(mp, &cbd, ents[i].data);
                    break;
                }
            }
            if (i == s->count && defFunc != NULL) {
                cbd.type = 6;
                cbd.x4 = _sceMpegBsGetPtr(bs, hp->x40);
                cbd.x8 = _sceMpegBsGetPtr(bs, hp->x38);
                cbd.xC = hp->x3C;
                cbd.x10 = hp->x28;
                cbd.x18 = hp->x30;
                ret = defFunc(mp, &cbd, defData);
            }
            if (ret != 0) {
                result = bs->pos >> 3;
            }
        }
        if (size * 8 < bs->pos || _sceMpegBsShowBits(bs, 32) != 0x1BA) {
            break;
        }
    }
    return result;
}
