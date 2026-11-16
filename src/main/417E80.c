/* rodata: 0x004A29C0-0x004A29E0 */
#include "common.h"
#include "main/n02.h"
#include "protos/main.h"

SprRectListEntry *MenuNumberLayout(MenuNumField *arg0, s32 arg1, f32 x, f32 y) {
    s8 n = arg0->digits;
    SprRectListEntry *s;
    s32 i;
    s16 sum;
    s8 *q;
    s32 r0;

    y = y - arg0->font->rects->h * arg0->sy;
    {
        s32 tbl[8] = { 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

        arg1 = (arg1 > -1) ? arg1 : -arg1;
        s = arg0->sprites;
        if (arg1 >= tbl[n - 1]) {
            x = x - (f32)(arg0->font->w[10] * n) * arg0->sx;
            i = n;
            if (i > 0) {
                do {
                    s->rect = arg0->font->rects + 10;
                    s->x = x;
                    s->y = y;
                    s++;
                    i--;
                    x += arg0->font->w[10];
                } while (i > 0);
            }
            s->x = x;
            s->rect = NULL;
            return s;
        } else {
            r0 = UtilIntToDigits(arg1, (u8 *)tbl);
            sum = 0;
            q = (s8 *)tbl + r0;
            {
                s32 j;
                s8 *r = q;
                for (j = r0; j < 8; j++) {
                    sum += arg0->font->w[*q++];
                }
                q = r;
            }
            x = x - sum * arg0->sx;
            for (; r0 < 8; r0++) {
                s->rect = arg0->font->rects + *q;
                s->x = x;
                s->y = y;
                s++;
                x += arg0->font->w[*q++];
            }
            s->x = x;
            s->rect = NULL;
            return s;
        }
    }
}

s16 MenuNumberWidth(MenuNumField *arg0, s32 arg1) {
    s32 tbl[8] = { 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
    s8 n = arg0->digits;
    s32 i;
    s16 sum;
    s8 *q;

    arg1 = (arg1 > -1) ? arg1 : -arg1;
    if (arg1 >= tbl[n - 1]) {
        return arg0->font->w[10] * n;
    }
    i = UtilIntToDigits(arg1, (u8 *)tbl);
    sum = 0;
    q = (s8 *)tbl + i;
    for (; i < 8; i++) {
        sum += arg0->font->w[*q++];
    }
    return sum;
}
