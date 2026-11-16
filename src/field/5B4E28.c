#include "common.h"
#include "protos/field.h"


/* s: one 0x5C-byte source record; pkt: 0x80-byte packet, p = its body at +0x20.
 * PF/PI go through a float/int union (alias set 0 in gcc 2.9, which orders the
 * scheduler like the original; a plain f32 PF is 55 words off); PG is a plain
 * f32 store. The source record is read at several widths per offset (its
 * layout depends on the kind), so it has no single struct. */
#define SB(o) (*(u8 *)(s + (o)))
#define SH(o) (*(u16 *)(s + (o)))
#define SS(o) (*(s16 *)(s + (o)))
#define SF(o) (*(f32 *)(s + (o)))
#define SW(o) (*(s32 *)(s + (o)))
#define PB(o) (*(u8 *)(p + (o)))
#define PH(o) (*(s16 *)(p + (o)))
typedef union { f32 f; s32 i; } FI;
#define PF(o) (((FI *)(p + (o)))->f)
#define PW(o) (*(s32 *)(p + (o)))
#define PG(o) (*(f32 *)(p + (o)))
#define PI(o) (((FI *)(p + (o)))->i)
#define KH(o) (*(u16 *)(pkt + (o)))
#define FLAGS KH(0x16)

void OccConvertRecords(u8 *base, s32 count) {
    s32 i;
    s32 j;
    u16 v;
    u8 *s;
    u8 *pkt;

    for (i = 0; i < count; i++) {
        s = base + i * 0x5C;
        pkt = OccGet(i);
        KH(0x2) = 0;
        KH(0x4) = SH(0x40);
        KH(0x18) = SH(0x44);
        KH(0x1A) = SH(0x56);
        KH(0x1C) = SH(0x58);
        KH(0x1E) = 0;
        KH(0x6) = SH(0x42);
        for (j = 0; j < 7; j++) {
            ((u16 *)(pkt + 8))[j] = ((u16 *)(s + 0x48))[j];
        }
        FLAGS = SH(0x46) & 7;
        if (SB(0x38)) FLAGS |= 0x100;
        if (SB(0x39)) FLAGS |= 0x200;
        if (SB(0x3A)) FLAGS |= 0x400;
        if (SB(0x3B)) FLAGS |= 0x800;
        switch (SS(0x44)) {
        case 0:
            {
            u8 *p = pkt + 0x20;
            PG(4) = SF(0x28);
            PH(0) = SH(0x3C);
            PH(2) = 1;
            PG(8) = SF(0x2C);
            PG(0xC) = SF(0x30);
            }
            break;
        case 1:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0x3C);
            PH(2) = SH(0x20);
            PH(4) = SB(0x1C);
            PH(6) = SH(0);
            PH(8) = SB(0x1D);
            PH(0xA) = SH(0xA);
            PF(0xC) = SS(0x14);
            PF(0x10) = SS(0x16);
            }
            break;
        case 2:
        case 4:
            {
            u8 *p = pkt + 0x20;
            if (FLAGS & 0x200) FLAGS |= 0x40;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SH(0x20);
            PH(6) = SH(0x22);
            PH(8) = SH(0x24);
            PF(0xC) = SF(0x28);
            PF(0x10) = SF(0x2C);
            PF(0x14) = SF(0x30);
            PH(0xA) = SH(0);
            PF(0x1C) = SF(0x34);
            PH(0x18) = SH(2);
            KH(0x1E) = SH(4);
            PH(0x20) = SH(6);
            PH(0x22) = SH(8);
            }
            break;
        case 3:
            {
            u8 *p = pkt + 0x20;
            if (FLAGS & 0x200) FLAGS |= 0x40;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SH(0x20);
            PH(6) = SH(0x22);
            PH(8) = SH(0x24);
            if (PH(2) == 0) {
                PI(0xC) = SF(0x28) * 182.04445f;
                PI(0x10) = SF(0x2C) * 182.04445f;
                PI(0x14) = SF(0x30) * 182.04445f;
            } else {
                PG(0xC) = SF(0x28);
                PG(0x10) = SF(0x2C);
                PG(0x14) = SF(0x30);
            }
            PH(0xA) = SH(0);
            PG(0x1C) = SF(0x34);
            PH(0x18) = SH(2);
            KH(0x1E) = SH(4);
            }
            break;
        case 5:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SH(0x20);
            PH(6) = SH(0x22);
            PH(8) = SH(0x24);
            PH(0xA) = SH(0);
            PH(0xC) = SH(2);
            PH(0xE) = SB(0x1E);
            PH(0x10) = SH(4);
            }
            break;
        case 6:
            {
            u8 *p = pkt + 0x20;
            if (FLAGS & 0x100) FLAGS |= 0x40;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SH(0x20);
            PH(8) = SH(0);
            PF(0xC) = SF(0x28);
            PH(0xA) = SH(2);
            KH(0x1E) = SH(6);
            }
            break;
        case 7:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SH(0x20);
            v = SH(4);
            if (v == 0) v = SH(0x22);
            PH(6) = v;
            PH(4) = SH(0);
            PH(8) = SH(2);
            }
            break;
        case 8:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1E);
            PH(2) = SB(0x1C);
            PH(4) = SH(0x20);
            PH(6) = SB(0x1D);
            PF(8) = SF(0x28);
            PH(0xC) = SH(0);
            }
            break;
        case 9:
            {
            u8 *p = pkt + 0x20;
            if (FLAGS & 0x100) FLAGS |= 0x40;
            PH(0) = SH(0x20);
            PH(2) = SH(0);
            PH(4) = SH(2);
            PH(6) = SH(4);
            }
            break;
        case 10:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0x20);
            PH(2) = SB(0x1C);
            PH(4) = SH(0x22);
            }
            break;
        case 11:
            {
            u8 *p = pkt + 0x20;
            FLAGS |= 0x40;
            PH(0) = SH(0x20);
            PH(2) = SH(0x22);
            PH(4) = SH(0x24);
            PH(6) = SB(0x1C);
            switch (SB(0x1C)) {
            case 0:
                PI(8) = SF(0x28) * 182.04445f;
                PI(0xC) = SF(0x2C) * 182.04445f;
                PI(0x10) = SF(0x30) * 182.04445f;
                break;
            case 1:
                PF(8) = SF(0x28);
                PF(0xC) = SF(0x2C);
                PF(0x10) = SF(0x30);
                break;
            case 2:
                PF(8) = SF(0x28);
                PF(0xC) = SF(0x2C);
                PF(0x10) = SF(0x30);
                break;
            }
            PH(0x14) = SH(0);
            switch (SB(0x1D)) {
            case 0:
            default:
                PH(0x16) = 6;
                break;
            case 1:
                PH(0x16) = 10;
                break;
            }
            PH(0x18) = SH(4);
            KH(0x1E) = SH(6);
            PH(0x1A) = SB(0x1E);
            }
            break;
        case 12:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SB(0x1E);
            PH(6) = SB(0x1F);
            PF(8) = SF(0x28);
            PH(0x12) = SH(0x14);
            PF(0xC) = SF(0x2C);
            }
            break;
        case 13:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0x20);
            PG(8) = SF(0x28);
            PH(2) = SH(0x22);
            PG(0xC) = SF(0x2C);
            PH(4) = SH(0x24);
            PG(0x10) = SF(0x30);
            PH(6) = SH(0x26);
            PG(0x14) = SF(0x34);
            PH(0x1C) = -1;
            }
            break;
        case 14:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SH(0x20);
            PG(4) = SS(0x14);
            PG(8) = SS(0x16);
            PG(0xC) = SS(0x18);
            PH(0x1C) = SH(0);
            PH(0x1E) = SH(2);
            PG(0x10) = SF(0x28);
            KH(0x1E) = SH(4);
            PG(0x14) = SF(0x2C);
            PH(0x20) = SH(6);
            PG(0x18) = SF(0x30);
            PH(0x22) = SH(8);
            }
            break;
        case 15:
            {
            u8 *p = pkt + 0x20;
            PB(0) = SB(0x20);
            PB(1) = SB(0);
            PB(2) = SB(2);
            PB(3) = SB(4);
            PB(4) = SB(6);
            PW(8) = SF(0x28) * 182.04445f;
            PW(0xC) = SF(0x2C) * 182.04445f;
            PW(0x10) = SF(0x30) * 182.04445f;
            PW(0x14) = SF(0x34) * 182.04445f;
            PB(5) = SB(0x1C);
            }
            break;
        case 16:
            {
            u8 *p = pkt + 0x20;
            PH(0x10) = SH(0);
            PH(0x12) = SH(2);
            FLAGS &= ~1;
            }
            break;
        case 17:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0x20);
            PH(2) = SH(6);
            PH(4) = SH(0);
            PH(6) = SB(0x1D);
            PH(8) = SB(0x1C);
            PF(0xC) = SF(0x28);
            PH(0x10) = SH(2);
            PH(0x12) = SH(4);
            PH(0x14) = SB(0x1E);
            }
            break;
        case 18:
            {
            u8 *p = pkt + 0x20;
            PH(2) = SH(0x20);
            PH(4) = SB(0x1C);
            PH(8) = SB(0x1D);
            PH(6) = SB(0x1E);
            PH(0) = SB(0x1F);
            }
            break;
        case 19:
            {
            u8 *p = pkt + 0x20;
            PF(0) = SS(0x14);
            PF(4) = SF(0x2C);
            PF(8) = SS(0x16);
            PF(0xC) = PF(0);
            PF(0x10) = SF(0x28);
            PF(0x14) = PF(8);
            PF(0x18) = SF(0x30);
            }
            break;
        case 20:
            {
            u8 *p = pkt + 0x20;
            PH(0x10) = SB(0x1C);
            PF(0) = SF(0x28);
            PH(0x12) = SH(0x20);
            PF(4) = SF(0x2C);
            PH(0x1E) = SH(0x22);
            PF(8) = SF(0x30);
            PH(0x18) = SH(0x14);
            PF(0x14) = SF(0x34);
            PH(0x1A) = SB(0x1E);
            PH(0x1C) = SH(0);
            }
            break;
        case 21:
            {
            u8 *p = pkt + 0x20;
            PH(0x54) = SB(0x1C);
            PG(0x10) = SF(0x28);
            PH(4) = SH(0);
            PG(0x18) = SF(0x2C);
            PH(6) = SH(2);
            PG(0x1C) = SF(0x30);
            PH(8) = SH(4);
            PG(0x14) = SF(0x34);
            PH(0xA) = SH(6);
            PW(0) = (PH(4) << 16) + (PH(6) << 8) + PH(8);
            PH(0xC) = SH(8);
            }
            break;
        case 22:
            {
            u8 *p = pkt + 0x20;
            if (FLAGS & 0x100) FLAGS |= 0x40;
                        PH(0) = SH(0x20);
            PI(4) = SW(0x3C);
            v = SH(4);
            PI(8) = SH(2);
            PH(2) = v;
            KH(0x1E) = SH(6);
            PI(0xC) = SH(8);
            }
            break;
        case 23:
            {
            u8 *p = pkt + 0x20;
            if (FLAGS & 0x200) FLAGS |= 0x40;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SH(0x20);
            PF(0xC) = SF(0x28);
            PF(0x10) = SF(0x2C);
            PF(0x14) = SF(0x30);
            PH(0xA) = SH(0);
            KH(0x1E) = SH(4);
            PH(0x1A) = SB(0x1E);
            }
            break;
        case 24:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0x20);
            PF(8) = SF(0x28);
            PH(2) = SH(0);
            PF(0xC) = SF(0x2C);
            PH(4) = SH(2);
            }
            break;
        case 25:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SH(0x20);
            PH(6) = SH(0);
            PH(8) = SH(0x22);
            PH(0xA) = SH(2);
            }
            break;
        case 26:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SH(2);
            PH(8) = SB(0x1E);
            PH(0xA) = SH(0);
            }
            break;
        case 27:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0);
            }
            break;
        case 28:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0);
            PH(2) = SB(0x1C);
            PF(8) = SF(0x28);
            PH(4) = SH(0x14);
            PF(0xC) = SF(0x2C);
            PH(6) = SH(0x16);
            PF(0x10) = SF(0x30);
            }
            break;
        case 29:
            {
            u8 *p = pkt + 0x20;
            FLAGS |= 0x40;
            PH(2) = SB(0x1C);
            PH(0) = SB(0x1D);
            PH(4) = SH(0x20);
            PH(6) = SH(0);
            PH(8) = SH(2);
            PH(0xA) = SH(4);
            PH(0xC) = SH(6);
            PH(0xE) = SH(8);
            PH(0x10) = SH(0xA);
            PH(0x12) = SH(0xC);
            }
            break;
        case 30:
            {
            u8 *p = pkt + 0x20;
            FLAGS |= 0x40;
            PB(0) = SB(0x20);
            PB(1) = SB(0);
            PB(2) = SB(2);
            PB(3) = SB(0x1C);
            PB(4) = SB(0x1D);
            PB(5) = SB(4);
            PB(6) = SB(6);
            PB(7) = SB(8);
            PB(8) = SB(0xA);
            PB(9) = SB(0xC);
            }
            break;
        case 31:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SB(0x1D);
            PH(4) = SB(0x1E);
            }
            break;
        case 32:
            {
            u8 *p = pkt + 0x20;
            PW(0) = SB(0x1C);
            PG(4) = SH(0);
            PG(8) = SS(0x14);
            PG(0xC) = SS(0x16);
            PG(0x10) = SH(2);
            PG(0x14) = SF(0x28);
            PG(0x18) = SF(0x2C);
            PG(0x1C) = SF(0x30);
            PG(0x20) = SF(0x34);
            PG(0x24) = SH(4);
            PG(0x28) = SB(0x1D);
            PW(0x2C) = SH(6);
            if (!SB(0x39)) PW(0x30) = 0; else PW(0x30) = 1;
            if (!SB(0x3A)) PW(0x34) = 0; else PW(0x34) = 1;
            PW(0x38) = SH(8);
            PW(0x3C) = SH(0xA);
            PW(0x40) = SH(0xC);
            PW(0x44) = SH(0xE);
            }
            break;
        case 33:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PF(0x10) = SF(0x28);
            PH(2) = SH(0x20);
            PF(0x14) = SF(0x2C);
            PH(4) = SH(0);
            PH(6) = SH(2);
            PH(8) = SH(4);
            PH(0xA) = SH(0x14);
            PH(0xC) = SH(0x16);
            }
            break;
        case 35:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SB(0x1C);
            PH(2) = SH(0x20);
            PB(4) = SB(2);
            PB(5) = SB(4);
            PB(6) = SB(6);
            PB(7) = SB(8);
            PB(8) = SB(0xA);
            PB(9) = SB(0xC);
            PB(0xA) = SB(0);
            PB(0xB) = SB(0x14);
            PB(0xC) = SB(0x16);
            PB(0xD) = SB(0x18);
            PB(0xE) = SB(0x1A);
            }
            break;
        case 36:
            {
            u8 *p = pkt + 0x20;
            PH(0) = SH(0);
            PH(2) = SH(2);
            PH(4) = SH(0x14);
            PH(6) = SH(4);
            PH(8) = SH(6);
            }
            break;
        case 39:
            {
            u8 *p = pkt + 0x20;
            PB(0) = SB(0x1C);
            PB(1) = SB(0x1D);
            PH(2) = SH(0x14);
            PG(8) = SF(0x28);
            PG(0xC) = SF(0x2C);
            PH(4) = SH(0x16);
            }
            break;
        case 40:
            {
            u8 *p = pkt + 0x20;
            PB(0x10) = SB(0x1C);
            PB(0x12) = SB(0x1D);
            PG(0) = SF(0x28);
            PH(0x18) = SH(0x14);
            PG(4) = SF(0x2C);
            PH(0x1A) = SH(0x16);
            PG(8) = SF(0x30);
            }
            break;
        case 41:
            {
            u8 *p = pkt + 0x20;
            PB(0x24) = SB(0x38);
            PF(0) = SF(0x28);
            PF(4) = SF(0x2C);
            PF(8) = SF(0x30);
            PW(0x10) = SS(0x14);
            PW(0x14) = SS(0x16);
            PW(0x18) = SS(0x18);
            }
            break;
        case 43:
            break;
        }
    }
    OccSetCount(count);
}
