#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "sys.h"


void BtlCaptionInit(void) {
    btlCaption.text = NULL;
    btlNotice.name = NULL;
    TxtGlyphBufInit(&btlCaptionGlyphBuf, sysFontImage2, 0x34, 0x63);
    TxtGlyphBufInit(&btlNoticeGlyphBuf, sysFontImage2, 0x11C, 0x143);
}
