#include "common.h"
#include "protos/field.h"


s32 FldBattleGetStageNo(u8 a) {
    u16 code = MapGetNo();

    if (code == 0x4030) {
        return 1;
    }
    if (code == 0x4040) {
        return 2;
    }
    if (code == 0x4050) {
        return a + 2;
    }
    if (code == 0x4800) {
        return 4;
    }
    if (code == 0x4810) {
        return a + 5;
    }
    if (code == 0x4820) {
        return 7;
    }
    if (code == 0x48F0) {
        return 8;
    }
    if (code == 0x5010) {
        return 9;
    }
    if (code == 0x5020) {
        return a ? 0x43 : 0xA;
    }
    if (code == 0x5024) {
        return 0xB;
    }
    if (code == 0x5030) {
        return 0xC;
    }
    if (code == 0x50F0) {
        return 0xD;
    }
    if (code == 0x5800) {
        return 0xE;
    }
    if (code == 0x5810) {
        return 0xF - a;
    }
    if (code == 0x5830) {
        return 0x10;
    }
    if (code == 0x58F0) {
        return 0x11;
    }
    if (code == 0x7000) {
        return 0x12;
    }
    if (code == 0x7010) {
        return 0x13;
    }
    if (code == 0x7020) {
        return 0x14;
    }
    if (code == 0x70F0) {
        return 0x15;
    }
    if (code == 0x8000) {
        return 0x16;
    }
    if (code == 0x8010) {
        return 0x17;
    }
    if (code == 0x8020) {
        return 0x18;
    }
    if (code == 0x80F0) {
        return 0x19;
    }
    if (code == 0x9000) {
        return 0x1A;
    }
    if (code == 0x9010) {
        return a + 0x1B;
    }
    if (code == 0x9014) {
        return 0x1D;
    }
    if (code == 0x9020) {
        return 0x1E;
    }
    if (code == 0x9040) {
        return 0x1F;
    }
    if (code == 0x9034 || code == 0x9038) {
        return 0x20;
    }
    if (code == 0x3838) {
        return 0x21;
    }
    if (code == 0x3839) {
        return 0x21;
    }
    if (code == 0x3830) {
        return 0x22;
    }
    if (code == 0x5A20) {
        return 0x23;
    }
    if (code == 0xA086) {
        return a + 0x24;
    }
    if (code == 0x6000) {
        return 0x26;
    }
    if (code == 0x6004) {
        return 0x27;
    }
    if (code == 0x6008) {
        return 0x28;
    }
    if (code == 0x600C) {
        return 0x29;
    }
    if (code == 0x6010) {
        return 0x2A;
    }
    if (code == 0x6014) {
        return 0x2B;
    }
    if (code == 0xA000) {
        return 0x2C;
    }
    if (code == 0xA004) {
        return 0x2D;
    }
    if (code == 0xA008) {
        return 0x2E;
    }
    if (code == 0xD800) {
        return 0x2F;
    }
    if (code == 0xD804) {
        return 0x30;
    }
    if (code == 0xD808) {
        return 0x31;
    }
    if (code == 0xD80C) {
        return 0x32;
    }
    if (code == 0xD810) {
        return 0x33;
    }
    if (code == 0xD814) {
        return 0x34;
    }
    if (code == 0xD818) {
        return 0x35;
    }
    if (code == 0xD81C) {
        return 0x36;
    }
    if (code == 0xD820) {
        return 0x37;
    }
    if (code == 0xD824) {
        return 0x38;
    }
    if (code == 0xD828) {
        return 0x39;
    }
    if (code == 0xD82C) {
        return 0x3A;
    }
    if (code == 0xD830) {
        return 0x3B;
    }
    if (code == 0xD834) {
        return 0x3C;
    }
    if (code == 0xD838) {
        return 0x3D;
    }
    if (code == 0xD83C) {
        return 0x3E;
    }
    if (code == 0xD840) {
        return 0x3F;
    }
    if (code == 0xD844) {
        return 0x40;
    }
    if (code == 0xD848) {
        return 0x41;
    }
    if (code == 0xD84C) {
        return 0x42;
    }
    return 0;
}
