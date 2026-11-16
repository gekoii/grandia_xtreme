#include "common.h"
#include "gph.h"

u32 ChrPartGetAttr(GphPart *n, u32 which) {
    u32 a;
    u32 b;

    if (n == NULL) {
        return 0;
    }
    a = n->attr;
    b = n->gsFlags;
    if (which < 7) {
        if (which == 0) {
            return (a & 0xE) >> 1;
        }
        if (which == 1) {
            return (a & 0x10) >> 4;
        }
        if (which == 2) {
            return (a & 0x20) >> 5;
        }
        if (which == 3) {
            return (a & 0x40) >> 6;
        }
        if (which == 4) {
            return (b & 0x2) >> 1;
        }
        if (which == 5) {
            return (b & 0x4) >> 2;
        }
        if (which == 6) {
            return (b & 0x8) >> 3;
        }
    } else {
        if (which == 7) {
            return (b & 0x10) >> 4;
        }
        if (which == 8) {
            return (b & 0xE0) >> 5;
        }
        if (which == 9) {
            return (b & 0x100) >> 8;
        }
        if (which == 10) {
            return (b & 0x600) >> 9;
        }
        if (which == 11) {
            return (b & 0x1800) >> 11;
        }
        if (which == 12) {
            return (b & 0x6000) >> 13;
        }
        if (which == 13) {
            return b >> 15;
        }
        if (which == 14) {
            return (a & 0x40) >> 6;
        }
    }
    return 0;
}
