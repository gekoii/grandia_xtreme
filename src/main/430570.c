/* rodata: 0x004A67E0-0x004A67EC */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[0x14];
    u16 unk14;
} Tex;

void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s64);

void GsTexSetupDraw(s32 arg0, Tex *arg1, void *arg2, u8 arg3, u8 arg4) {
    u8 tbl[3][4] = { { 0, 1, 0, 1 }, { 0, 2, 0, 1 }, { 2, 0, 0, 1 } };

    if (arg0 != 0 && arg1 != NULL) {
        GsTexLoadPacket(arg0, arg1, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (arg2 != NULL) {
        s32 tw;
        s32 fix;
        s32 px;
        s32 flag;
        s64 o1;
        s64 o2;
        arg3 &= 3;
        tw = arg1 != NULL ? arg1->unk14 & 0xF : 0;
        o1 = 1;
        o2 = 1;
        if (arg3 != 0) {
            fix = 0;
            px = 0x80;
        } else {
            fix = 6;
            px = 0x80;
        }
        flag = 0;
        if (arg4) {
            flag = 1;
        }
        GsRenderStatePacketSet(arg2, 1, 1, flag, 1, 0, 0, 0,
                      0, 1, fix, px, 1, 0, 0, 1, 2,
                      tbl[arg3][0], tbl[arg3][1], tbl[arg3][2], tbl[arg3][3],
                      0, 0, o1, tw, 1, o2, 0, 0, 0);
    }
}
