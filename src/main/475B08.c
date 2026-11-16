/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"


typedef struct {
    s32 x0, y0, x1, y1;
} Rect;

typedef struct {
    s32 x, y;
} Pt;

#define HALF(v) ((v) > 0 ? ((v) + 1) >> 1 : (v) >> 1)
#define HALF3(v) (((v) * 3 + ((v) > 0 ? 1 : 0)) >> 1)

void _calcDMV(Rect *r, Pt *p, s32 w, s32 h) {
    if (picture_structure == 3) {
        if (top_field_first != 0) {
            r->x0 = HALF(w) + p->x;
            r->y0 = HALF(h) + p->y - 1;
            r->x1 = HALF3(w) + p->x;
            r->y1 = HALF3(h) + p->y + 1;
        } else {
            r->x0 = HALF3(w) + p->x;
            r->y0 = HALF3(h) + p->y - 1;
            r->x1 = HALF(w) + p->x;
            r->y1 = HALF(h) + p->y + 1;
        }
        return;
    }
    r->x0 = HALF(w) + p->x;
    r->y0 = HALF(h) + p->y;
    if (picture_structure == 1) {
        r->y0 = r->y0 - 1;
    } else {
        r->y0 = r->y0 + 1;
    }
}
