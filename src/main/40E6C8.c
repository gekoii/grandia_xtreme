/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "sys.h"
#include "protos/main.h"
#include "gph.h"

void GphFbEffectPass(void) {
    if (gphFbDivParam.blur != 0) {
        GphFbBlur(0, 0, 0x280, 0x1C0, 0, gphFbDivParam.blur);
    }
    if (gphFbDivParam.bokeh > 0.02f) {
        GphFbBokeh(0, 0, 0x280, 0x1C0, gphFbDivParam.bokeh);
    }
    if (gphFbDivParam.blow > 0.001f) {
        GphFbMeshWarp(0, 0, 0x280, 0x1C0, gphFbDivParam.blow);
    }
    if (gphFbEffParam[0] > 0) {
        GphFbVWaveSet(0, gphFbEffParam[0], gphFbEffParam[1], gphFbEffParam[2], gphFbEffParam[3],
                      gphFbEffParam[4], gphFbEffParam[5], gphFbEffParam[6], gphFbEffParam[7]);
        GphFbVWaveDraw(0, 0, 0x280, 0x1C0);
    }
    if (gphFbDivParam.multiMode != 0) {
        GphFbMultiScreen(0, 0, 0x280, 0x1C0, gphFbDivParam.multiW, gphFbDivParam.multiH);
    }
    if (sysState.unk60 != 0) {
        sysState.unk60--;
        GphFbOverlayImage(0, 0, 0x280, 0x1C0, sysState.unk60, sysState.screenCaptureAddr);
    }
    if (gphFbDivParam.rippleTrigger != 0) {
        gphFbDivParam.ripplePhase += 0.75398207f;
        if (gphFbDivParam.ripplePhase > 62.83184f) {
            gphFbDivParam.ripplePhase = 0.0f;
            gphFbDivParam.rippleTrigger = 0;
        }
        GphFbRipple(gphFbDivParam.ripplePhase, 320.0f, 224.0f, 100.0f, 4.0f);
    }
    if (gphFbDivParam.centerView != 0.0f) {
        GphFbCenterView();
    }
    if (gphFbDivParam.nineDivide != 0.0f) {
        GphFbNineDivide();
    }
    if (gphFbDivParam.nineDivScrollV != 0.0f) {
        GphFbNineDivideScrollV(gphFbDivParam.nineDivScrollV);
    }
    if (gphFbDivParam.nineDivScrollH != 0.0f) {
        GphFbNineDivideScrollH(gphFbDivParam.nineDivScrollH);
    }
    if (gphFbDivParam.mirror != 0.0f) {
        GphFbMirror();
    }
}
