#ifndef FIELD_N18_H
#define FIELD_N18_H

/* FIELD text helpers (0x005E9F18-0x005EA578) and the PARAM DEBUG pages
 * (0x005ED820-0x005EFBD8). The item module between them (Item*) uses
 * itemWork from field/menu.h and the party data from party.h. */

#include "common.h"

/* ---------------------------------------------------------------- text */

/* Header of a message bank: a length table and a string-pointer table,
 * both stored as offsets from the header. String i is lenTable[i] glyph
 * codes (u16) at strTable[i] (TxtMsgGetLength 5E9FA0, TxtMsgGetWidth
 * 5E9FB8, TxtMsgGetCodes 5EA208, TxtGlyphQueueMsg 5EA2E8,
 * TxtMsgSpritesBuild 5EA4D0). */
typedef struct TxtMsgHdr {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 lenOfs;   /* -> u8 lengths[], one per string */
    /* 0xC */ s32 strOfs;   /* -> u16 *strings[] */
} TxtMsgHdr; /* size 0x10 (header only) */

/* ---------------------------------------------------------- PARAM DEBUG */

extern s32 dbgParamX;  /* 0x00666CE8, text column of the pages (3) */
extern s32 dbgParamY;  /* 0x00666CEC, text row of the pages (4) */

#endif /* FIELD_N18_H */
