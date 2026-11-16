/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniSpritesUpdate(MiniSprite *arr, u32 n) {
    u32 i;
    u8 fl;
    u8 f2;
    s8 lim;
    SprRect *src;
    SprRect *dst;
    f32 f;
    f32 sx;
    f32 sy;

    for (i = 0; i < n; i++) {
        if (arr[i].active == 0) {
            continue;
        }
        arr[i].frame++;
        if (arr[i].animNo != 0) {
            GphAnimSetSelect(miniLayout->images[arr[i].animNo + miniLayout->hdr->texCount - 1]);
            GphAnimStep(&arr[i].anim, 1.0f);
        }
        fl = arr[i].moveFlags;
        if (fl != 0) {
            if (fl & 1) {
                arr[i].x = arr[i].startX + (arr[i].goalX - arr[i].startX) * ((f32)(arr[i].moveCount + 1) / (f32)arr[i].moveTime);
                arr[i].y = arr[i].startY + (arr[i].goalY - arr[i].startY) * ((f32)(arr[i].moveCount + 1) / (f32)arr[i].moveTime);
            }
            if (fl & 4) {
                arr[i].sx += (1.0f - arr[i].sx) / (f32)(arr[i].moveTime - arr[i].moveCount);
                arr[i].sy += (1.0f - arr[i].sy) / (f32)(arr[i].moveTime - arr[i].moveCount);
                arr[i].ofsY = (1.0f - arr[i].sy) * (f32)GphAnimGetCellRect(arr[i].anim.cell)->h;
            }
            if (++arr[i].moveCount == arr[i].moveTime) {
                u8 fr = arr[i].moveFlags;
                arr[i].moveCount = 0;
                if (fr & 2) {
                    arr[i].animNo = arr[i].rect;
                    arr[i].rect = 0;
                }
                if (fr & 8) {
                    arr[i].active = 0;
                }
                arr[i].moveFlags = 0;
            }
        }
        if (arr[i].fxFlags == 0) {
            continue;
        }
        if (arr[i].fxFlags & 0x10) {
            f = ((f32)arr[i].stretchT / 100.0f) * (f32)arr[i].stretchSpeed;
            src = &miniLayout->rects[arr[i].rect];
            dst = &miniLayout->rects[arr[i].stretchRect];
            switch (arr[i].stretchMode) {
            case 0:
                dst->v0 = (f32)src->v0 + f;
                dst->v1 = (f32)src->v1 + f;
                if ((((src->v1 < dst->v0) || (dst->v1 < src->v0)) && (src->v0 < src->v1))
                    || (((src->v0 < dst->v1) || (dst->v0 < src->v1)) && (src->v1 < src->v0))) {
                    dst->v0 = src->v0;
                    dst->v1 = src->v1;
                    arr[i].stretchT = 0;
                }
                break;
            case 1:
                dst->u0 = (f32)src->u0 + f;
                dst->u1 = (f32)src->u1 + f;
                if ((((src->u1 < dst->u0) || (dst->u1 < src->u0)) && (src->u0 < src->u1))
                    || (((src->u0 < dst->u1) || (dst->u0 < src->u1)) && (src->u1 < src->u0))) {
                    dst->u0 = src->u0;
                    dst->u1 = src->u1;
                    arr[i].stretchT = 0;
                }
                break;
            case 2:
                arr[i].angle = f;
                if (f > 65535.0f || f < -65535.0f) {
                    arr[i].angle = 0;
                    arr[i].stretchT = 0;
                }
                break;
            }
            arr[i].stretchT++;
        }
        if (arr[i].fxFlags & 2) {
            sx = arr[i].sx;
            if (sx > 1.2f) {
                arr[i].pulseX = 99;
            }
            if (sx < 1.0f) {
                arr[i].pulseX = 101;
            }
            sy = arr[i].sy;
            arr[i].sx = sx * ((f32)arr[i].pulseX / 100.0f);
            if (sy > 1.2f) {
                arr[i].pulseY = 99;
            }
            if (sy < 1.0f) {
                arr[i].pulseY = 101;
            }
            arr[i].sy = sy * ((f32)arr[i].pulseY / 100.0f);
        }
        if (arr[i].fxFlags & 0x20) {
            sx = arr[i].sx;
            if (sx > 1.2f) {
                arr[i].pulseX = 99;
            }
            if (sx < 1.0f) {
                arr[i].sx = 1.0f;
                arr[i].pulseX = 100;
                sx = 1.0f;
                arr[i].pulseY = 101;
            }
            sy = arr[i].sy;
            arr[i].sx = sx * ((f32)arr[i].pulseX / 100.0f);
            if (sy > 1.2f) {
                arr[i].pulseY = 99;
            }
            if (sy < 1.0f) {
                arr[i].sy = 1.0f;
                arr[i].pulseY = 100;
                sy = 1.0f;
                arr[i].pulseX = 101;
            }
            arr[i].sy = sy * ((f32)arr[i].pulseY / 100.0f);
        }
        if (arr[i].fxFlags & 0x40) {
            arr[i].sx = arr[i].sx * ((f32)arr[i].pulseX / 100.0f);
            arr[i].sy = arr[i].sy * ((f32)arr[i].pulseY / 100.0f);
            if (arr[i].sx < 0.001f) {
                arr[i].sx = 0.0f;
            }
            if (arr[i].sy < 0.001f) {
                arr[i].sy = 0.0f;
            }
        }
        f2 = arr[i].fxFlags;
        if (f2 & 0xC) {
            lim = arr[i].wavePeriod;
            if (++arr[i].waveT > lim) {
                arr[i].waveT = 0;
            }
            if (f2 & 4) {
                arr[i].waveX = arr[i].waveAmpX * MiniTriangleWave((f32)arr[i].waveT / (2.0f * (f32)lim));
                f2 = arr[i].fxFlags;
            }
            if (f2 & 8) {
                arr[i].waveY = arr[i].waveAmpY * MiniTriangleWave((f32)arr[i].waveT / (2.0f * (f32)arr[i].wavePeriod));
            }
        }
        if (arr[i].fxFlags & 0x80) {
            arr[i].alphaScale = MiniTriangleWave((f32)(arr[i].frame % arr[i].blinkPeriod) / (f32)arr[i].blinkPeriod) * 128.0f;
        }
    }
}
