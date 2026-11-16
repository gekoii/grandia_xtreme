#include "common.h"
#include "gph.h"

void ChrPartSetAttr(GphPart *n, u32 which, s32 v) {
    u32 a;
    u32 b;

    if (n == NULL) {
        return;
    }
    a = n->attr & ~1;
    b = n->gsFlags & ~1;
    switch (which) {
    case 0:
        a &= 0xFFFFFFF1;
        a |= ((v & 7) << 1);
        break;
    case 1:
        if (v == 0) {
            a &= 0xFFFFFFEF;
        }
        if (v == 1) {
            a |= 0x10;
        }
        break;
    case 2:
        if (v == 0) {
            a &= 0xFFFFFFDF;
        }
        if (v == 1) {
            a |= 0x20;
        }
        break;
    case 3:
        if (v == 0) {
            a &= 0xFFFFFFBF;
        }
        if (v == 1) {
            a |= 0x40;
        }
        break;
    case 4:
        if (v == 0) {
            b &= 0xFFFFFFFD;
        }
        if (v == 1) {
            b |= 0x2;
        }
        break;
    case 5:
        if (v == 0) {
            b &= 0xFFFFFFFB;
        }
        if (v == 1) {
            b |= 0x4;
        }
        break;
    case 6:
        if (v == 0) {
            b &= 0xFFFFFFF7;
        }
        if (v == 1) {
            b |= 0x8;
        }
        break;
    case 7:
        if (v == 0) {
            b &= 0xFFFFFFEF;
        }
        if (v == 1) {
            b |= 0x10;
        }
        break;
    case 8:
        b &= 0xFFFFFF1F;
        b |= ((v & 7) << 5);
        break;
    case 9:
        if (v == 0) {
            b &= 0xFFFFFEFF;
        }
        if (v == 1) {
            b |= 0x100;
        }
        break;
    case 10:
        b &= 0xFFFFF9FF;
        b |= ((v & 3) << 9);
        break;
    case 11:
        b &= 0xFFFFE7FF;
        b |= ((v & 3) << 11);
        break;
    case 12:
        b &= 0xFFFF9FFF;
        b |= ((v & 3) << 13);
        break;
    case 13:
        if (v == 0) {
            b &= 0xFFFF7FFF;
        }
        if (v == 1) {
            b |= 0x8000;
        }
        break;
    case 14:
        if (v == 0) {
            a &= 0xFFFFFF7F;
        }
        if (v == 1) {
            a |= 0x80;
        }
        break;
    }
    if (a & 0xFFFE) {
        a |= 1;
    }
    if ((b & 0xFFFE) != 0xA7B2) {
        b |= 1;
    }
    n->attr = a;
    n->gsFlags = b;
}
