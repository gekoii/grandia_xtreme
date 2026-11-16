#include "common.h"

s32 EvtMapFlagSlot(u16 arg0) {
    s32 lo = arg0 & 0xFF;

    arg0 >>= 8;
    switch (arg0) {
    case 0x40:
    case 0x48:
    case 0x50:
    case 0x58:
    case 0x70:
    case 0x80:
    case 0x90:
        if (lo == 0x00) return 0;
        if (lo == 0x10) return 1;
        if (lo == 0x20) return 2;
        if (lo == 0x24) return 3;
        if (lo == 0x30) return 4;
        if (lo == 0x40) return 5;
        if (lo == 0x50) return 6;
        if (lo == 0xA0) return 7;
        if (lo == 0xF0) return 8;
        break;
    }
    return -1;
}
