/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"


s32 sceMpegClearRefBuff(void) {
    if (_sceMpegFwdRefFrame != NULL) {
        _sceMpegFwdRefFrame->ref = 0;
    }
    if (_sceMpegFwdRefTop != NULL) {
        _sceMpegFwdRefTop->ref = 0;
    }
    if (_sceMpegFwdRefBottom != NULL) {
        _sceMpegFwdRefBottom->ref = 0;
    }
    if (_sceMpegBwdRefFrame != NULL) {
        _sceMpegBwdRefFrame->ref = 0;
    }
    if (_sceMpegBwdRefTop != NULL) {
        _sceMpegBwdRefTop->ref = 0;
    }
    if (_sceMpegBwdRefBottom != NULL) {
        _sceMpegBwdRefBottom->ref = 0;
    }
    return 1;
}
