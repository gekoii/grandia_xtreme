#include "common.h"
#include "sys.h"
#include "protos/main.h"
#include "gph.h"

void GphShatterStart(u8 mode) {
    s32 one = 1;

    gphShatter.mode = mode;
    gphShatter.unkC = 0.0f;
    gphShatter.unk10 = 0.0f;
    gphFbDivParam.blur = 0;
    gphShatter.unk8 = 0;
    sysScreenCaptureReq = one;
    gphShatter.color[3] = 127.0f;
    gphFbDivParam.system2Mode = 0;
    gphShatter.active = one;
    gphShatter.timer = 0;
    switch (mode) {
    case 0:
        GphShatterScatterMode0();
        break;
    case 1:
        GphShatterScatterMode1();
        gphFbDivParam.blur = 0x5A;
        break;
    case 3:
        gphShatter.timer = 4;
        gphFbDivParam.system2Mode = 1;
        GphShatterScatterMode3();
        break;
    case 9:
    case 10:
        GphShatterScatterMode9();
        break;
    }
}
