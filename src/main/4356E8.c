#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"


s32 GphPartMorph(s32 arg0, GphPart *arg1, GphMotCursor *arg2) {
    s32 *p;
    GphMotTrackHdr *h;
    f32 f;
    u16 flags;

    f = arg2->frame;
    p = arg2->track;
    h = (GphMotTrackHdr *)p;
    p++;
    flags = h->flags;
    if (flags == 0) {
        return 0;
    }
    if (flags & 1) p++;
    if (flags & 2) p++;
    if (flags & 4) p++;
    if (flags & 8) p++;
    if (flags & 0x10) {
        GphMorphPositions(arg1, *p, f);
        p++;
    }
    if (flags & 0x20) {
        GphMorphNormals(arg1, *p, f);
    }
    return 0;
}
