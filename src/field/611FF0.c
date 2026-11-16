#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n21.h"


void AmapOccAdd(AmapWork *arg0, u32 arg1, s16 arg2, s32 arg3, s32 arg4) {
    OccEntry *s;
    s32 i;
    f32 f;

    s = &arg0->occ[arg0->occCount];
    s->state = 0;
    s->id = arg0->occCount;
    s->unk6 = arg2;
    s->unk1A = 0;
    s->unk1C = 0;
    s->unk1E = 0;
    for (i = 6; i >= 0; i--) {
        ((s16 *)&s->cond)[i] = 0;
    }
    s->flags = 0;

    switch (arg1) {
    case 0x0:
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, 36.0f, 0.0f, 10, 120, -1);
        break;
    case 0x1:
        s->unk1A = 200;
        AmapOccSetMapJump(s, 0x2000, arg0->mapNo);
        break;
    case 0x2:
        s->unk1A = 200;
        AmapOccSetMapJump(s, arg4, arg0->mapNo);
        break;
    case 0x3:
        s->unk1A = 200;
        s->kind = 0x2B;
        break;
    case 0x4:
        AmapOccSetScript(s, arg4 - 0x1FFF);
        break;
    case 0x6:
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, -1.0f, 0.0f, 500, 240, -1);
        break;
    case 0x7:
        AmapOccSetTrap(s, arg0->trapArg1[2], arg0->trapArg2[2]);
        break;
    case 0x8:
        AmapOccSetStateSet(s, 0, arg4, 1);
        break;
    case 0x9:
        AmapOccSetCondFlag(s, 4, 1);
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, 36.0f, 0.0f, 10, 0, -1);
        break;
    case 0xA:
        AmapOccSetTrap(s, arg0->trapArg1[0], arg0->trapArg2[0]);
        break;
    case 0xB:
        AmapOccSetTrap(s, arg0->trapArg1[1], arg0->trapArg2[1]);
        break;
    case 0x11:
        AmapOccSetSwitch(s, arg3, 6, 1, 0);
        break;
    case 0x13:
        AmapOccSetChrFlags(s, arg3, 1, 0);
        break;
    case 0x14:
        s->flags = 2;
    case 0x15:
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, 36.0f, 0.0f, 10, 0, -1);
        break;
    case 0x16:
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, -36.0f, 0.0f, 10, 0, 0xDAD);
        break;
    case 0x17:
        s->unk1A = 60;
        AmapOccSetChrFlags(s, arg3, 1, 1);
        break;
    case 0x19:
        s->flags = 2;
        AmapOccSetCondFlag(s, arg4 + 6, 1);
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, 36.0f, 0.0f, 10, 0, 0xDAC);
        break;
    case 0x1A:
        s->flags = 2;
        AmapOccSetSwitchEx(s, arg3, 0, arg4, arg4, 0, 0);
        break;
    case 0x1D:
        s->flags = 2;
        AmapOccSetMove(s, arg3, 1, 4, 0.0f, -1000.0f, 0.0f, 1, 0, -1);
        break;
    case 0x28:
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, 1000.0f, 0.0f, 1, 0, -1);
        break;
    case 0x2A:
        AmapOccSetChrMove(s, arg3, 0.0f, 0.0f, 0.0f);
        break;
    case 0x2B:
        AmapOccSetChrFlags(s, arg3, 1, 1);
        break;
    case 0x32:
        f = -1.0f;
        if (arg4 == 2) {
            f = 1.0f;
        }
        AmapOccSetMove(s, arg3, (u8)arg4, (u8)arg4, 0.0f, 0.0f, f, 40, 200, -1);
        break;
    case 0x33:
        s->unk1A = 40;
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, 9.0f, 0.0f, 40, 120, 0xDAC);
        break;
    case 0x35:
        s->unk1A = 0x118;
        AmapOccSetSwitch(s, arg3, 1, 1, 0);
        break;
    case 0x3C:
        s->flags = 2;
        AmapOccSetSwitch(s, arg3, 1, 0, 0);
        break;
    case 0x3D:
        s->unk1A = 40;
        s->flags = 2;
        AmapOccSetCondFlag2(s, 2, 4, 5, 1, 0);
        AmapOccSetSwitch(s, arg3, 1, 1, 0);
        break;
    case 0x3E:
        s->flags = 2;
        AmapOccSetCondFlag(s, arg4, 1);
        AmapOccSetMove(s, arg3, arg4 + 1, arg4 + 1, 0.0f, 0.0f, 1.0f, 40, 0, 0xDB2);
        break;
    case 0x3F:
        AmapOccSetMove(s, arg3, (u8)arg4, (u8)arg4, 0.0f, 0.0f, -1.0f, 40, 0, -1);
        break;
    case 0x40:
        s->unk1A = 40;
        AmapOccSetMove(s, arg3, 0, 0, 0.0f, 9.0f, 0.0f, 40, 0, 0xDAE);
        break;
    case 0x42:
        AmapOccSetStateSet(s, 0, arg3, 1);
        break;
    case 0x46:
        AmapOccSetUvScroll(s, arg3, 1, 0, 20, 0);
        break;
    case 0x47:
        s->unk1A = 100;
        AmapOccSetUvScroll(s, arg3, 1, 0, 0, 0);
        break;
    case 0x48:
        AmapOccSetUvScroll(s, arg3, (u8)(arg4 + 1), 0, 20, 0);
        break;
    case 0x49:
        s->unk1A = 100;
        AmapOccSetUvScroll(s, arg3, (u8)(arg4 + 1), 0, 0, 0);
        break;
    case 0x50:
        AmapOccSetStateSet(s, 1, 0, 1);
        break;
    case 0x51:
        AmapOccSetSwitchEx(s, arg3, 0, arg4, arg4, 0, 0);
        break;
    case 0x52:
        AmapOccSetUvScroll(s, arg3, 2, 0, 20, 0);
        break;
    case 0x53:
        s->unk1A = 100;
        AmapOccSetUvScroll(s, arg3, 2, 0, 0, 0);
        break;
    case 0x54:
        s->unk1A = 100;
        switch (arg0->mapType) {
        case 0:
        case 3 ... 7:
            AmapOccSetMove(s, arg3, 0, 0, 0.0f, -1.0f, 0.0f, 500, 240, 0xDB5);
            break;
        default:
            AmapOccSetMove(s, arg3, 0, 0, 0.0f, 1.0f, 0.0f, 500, 240, 0xDB5);
            break;
        }
        break;
    case 0x5A:
        s->flags = 2;
        switch (arg0->mapType) {
        case 0:
        case 3 ... 7:
            AmapOccSetMove(s, arg3, 0, 0, 0.0f, 150.0f, 0.0f, 1, 0, 0xDB5);
            break;
        default:
            AmapOccSetMove(s, arg3, 0, 0, 0.0f, -150.0f, 0.0f, 1, 0, 0xDB5);
            break;
        }
        break;
    case 0x5B:
        s->flags = 2;
        switch (arg0->mapType) {
        case 0:
        case 3 ... 7:
            AmapOccSetMove(s, arg3, 0, 0, 0.0f, -2.0f, 0.0f, 75, 0, 0xDB5);
            break;
        default:
            AmapOccSetMove(s, arg3, 0, 0, 0.0f, 2.0f, 0.0f, 75, 0, 0xDB5);
            break;
        }
        break;
    case 0x5C:
        s->flags = 2;
        s->unk1A = 0xAF;
        AmapOccSetMove(s, arg3, arg4, arg4, 0.0f, -2.0f, 0.0f, 20, 0, 0xDB3);
        break;
    case 0x5D:
        s->flags = 2;
        AmapOccSetUvScroll(s, arg3, 2, 0, 0x800, 0);
        break;
    case 0x5E:
        s->flags = 2;
        s->unk1A = 1;
        AmapOccSetUvScroll(s, arg3, 2, 0, 0, 0);
        break;
    case 0x5F:
        s->flags = 2;
        s->unk1A = 75;
        AmapOccSetUvScroll(s, arg3, 2, 0, 20, 0);
        break;
    case 0x60:
        s->flags = 2;
        s->unk1A = 0xAF;
        AmapOccSetUvScroll(s, arg3, 2, 0, 0, 0);
        break;
    case 0x61:
        s->flags = 2;
        AmapOccSetStateSet(s, 1, 0, 1);
        break;
    case 0x62:
        s->flags = 2;
        s->unk1A = 75;
        AmapOccSetStateSet(s, 1, 0, 0);
        break;
    case 0x63:
        s->flags = 2;
        s->unk1A = 0xC3;
        AmapOccSetSwitch(s, arg3, 1, 0, 0);
        break;
    case 0x64:
        AmapOccSetMapEffect(s, arg3, 10, 0x31);
        break;
    case 0x68:
        AmapOccSetMapEffect(s, arg3, 10, 0x32);
        break;
    case 0x65:
        AmapOccSetMapEffect(s, arg3, 10, 0x33);
        break;
    case 0x66:
    case 0x67:
        AmapOccSetMapEffect(s, arg3, 1, 0x34);
        break;
    case 0x6E:
        AmapOccSetSwitch(s, arg3, 1, 1, 0);
        break;
    case 0x78:
        AmapOccSetSwitch(s, arg3, 0, 0, 0);
        break;
    case 0x79:
        s->flags = 2;
        AmapOccSetSwitch(s, arg3, 6, 0, 0);
        break;
    case 0x7A:
        AmapOccSetSwitch(s, arg3, 6, 1, 0);
        break;
    case 0x7B:
        AmapOccSetMotion(s, arg3);
        break;
    case 0x7C:
        s->unk1A = 40;
        AmapOccSetChrFlag(s, arg3, 2, 0);
        break;
    case 0x7D:
        s->unk1A = 40;
        AmapOccSetChrFlagB(s, arg3, 2, 0);
        break;
    case 0x82:
        s->flags = 2;
        AmapOccSetSwitch(s, arg3, 1, arg4, 0);
        break;
    case 0x83:
        AmapOccSetSwitch(s, arg3, 1, arg4, 0);
        break;
    case 0x84:
        AmapOccSetSwitch(s, arg3, 1, arg4, 0);
        break;
    case 0x96:
        s->flags = 2;
        AmapOccSetSwitch(s, arg3, 1, 0, 0);
        break;
    case 0x97:
        s->flags = 2;
        s->unk1A = 75;
        AmapOccSetSwitch(s, arg3, 1, 1, 0);
        break;
    case 0x18:
    case 0x29:
    case 0x34:
    case 0x41:
    case 0x98:
        AmapOccSetSwitch(s, arg3, 1, 0, 0);
        break;
    case 0xA0:
        s->flags = 2;
        s->unk1A = 75;
        AmapOccSetPartFade(s, arg3, 4, 11, -2, 0x40);
        break;
    case 0xA1:
        s->unk1A = 75;
        AmapOccSetPartFade(s, arg3, 4, 11, 2, 0x40);
        break;
    case 0xA2:
        s->flags = 2;
        AmapOccSetPartFade(s, arg3, 4, 11, -0x80, 1);
        break;
    case 0xA5:
        s->flags = 2;
        s->unk1A = 75;
        AmapOccSetPartFade(s, arg3, 2, 2, -2, 0x40);
        break;
    case 0xA6:
        s->unk1A = 75;
        AmapOccSetPartFade(s, arg3, 2, 2, 2, 0x40);
        break;
    case 0xA7:
        s->flags = 2;
        AmapOccSetPartFade(s, arg3, 2, 2, -0x80, 1);
        break;
    case 0xAA:
        s->flags = 2;
        AmapOccSetPartFade(s, arg3, arg4, arg4 + 1, -0xFF, 1);
        break;
    case 0xAB:
        AmapOccSetPartFade(s, arg3, arg4, arg4 + 1, 4, 0x40);
        break;
    case 0xAC:
        s->flags = 2;
        AmapOccSetPartFade(s, arg3, arg4 + 1, arg4 + 1, -0x80, 1);
        break;
    case 0xAD:
        AmapOccSetPartFade(s, arg3, arg4 + 1, arg4 + 1, 2, 0x40);
        break;
    case 0xB4:
        s->flags |= 0x40;
        AmapOccSetScale(s, arg3, -1, -1);
        break;
    case 0xB5:
        s->flags |= 0x40;
        AmapOccSetMoveEx(s, arg3, 1, 0, 0, 0.0f, 360.0f, 0.0f, 1, 0, 0xEB0, 0xEB1);
        break;
    case 0xFF:
        AmapOccSetSound(s, arg4);
        break;
    }
    arg0->occCount++;
}
