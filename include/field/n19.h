/*
 * FIELD item, equipment and skill rules (0x005EFBD8-0x005F7570) and area
 * light transitions (0x005F7570-0x005F7D98): the tables that are not part
 * of party.h / field/menu.h (the Item* list records are in field/menu.h).
 *
 * Layouts were merged from the per-file views by byte offset.
 */
#ifndef FIELD_N19_H
#define FIELD_N19_H

#include "common.h"
#include "party.h"
#include "field/menu.h"

/* ---------------------------------------------------------------- area lights */

/* Directional light of a preset, 0x20 bytes (copied to GphLightGet(i + 1)). */
typedef struct FldLightPresetDir {
    /* 0x00 */ f32 color[4];       /* -> GphLight.color */
    /* 0x10 */ f32 angleX;         /* degrees, VuDirFromAngles -> GphLight.dir */
    /* 0x14 */ f32 angleY;
    /* 0x18 */ f32 intensity;      /* -> GphLight.intensity */
    /* 0x1C */ f32 unk1C;
} FldLightPresetDir; /* size 0x20 */

/* Map light preset, 0xC0 bytes (fldLightPresetTable, map header +0x12C). */
typedef struct FldLightPreset {
    /* 0x00 */ FldLightPresetDir dirs[4]; /* the first 2 are used */
    /* 0x80 */ f32 ambient[4];     /* -> GphLightGet(0)->color */
    /* 0x90 */ f32 fogColor[3];    /* -> camField[0].fogColor */
    /* 0x9C */ u8 pad9C[4];
    /* 0xA0 */ f32 fogMax;
    /* 0xA4 */ f32 fogDist;
    /* 0xA8 */ f32 fogMin;
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ f32 clipFar;
    /* 0xB4 */ u8 padB4[0xC];
} FldLightPreset; /* size 0xC0 */

/* Fade between two presets (fldLightFade, FldLightFadeUpdate), 0xD0 bytes. */
typedef struct FldLightFade {
    /* 0x00 */ u8 pad0[0xC0];      /* cleared by FldLightFadeInit, not used in C */
    /* 0xC0 */ s32 active;
    /* 0xC4 */ u16 from;           /* preset indices; s16 in 0x005F7990 */
    /* 0xC6 */ u16 to;
    /* 0xC8 */ f32 t;              /* 0..1; 0x005F7990 clears it as s32 */
    /* 0xCC */ f32 frames;
} FldLightFade; /* size 0xD0 */

/* ---------------------------------------------------------------- globals */

extern s16 itemEggCapacity;        /* 0x00666CD6, 12 or 24 (5EAD80) */
extern s16 itemBookCapacity;       /* 0x00666CD8, 12 or 24 */
extern s16 itemSkillCapacity;      /* 0x00666CDA, 50 or 100 */
extern u8 itemEquipSlotOrder[6];   /* 0x00666CB8, display slot -> PtyChar.equip index */
extern u8 itemEggChartCellTable[]; /* 0x00666E38, mana egg type -> egg chart cell */
extern FldLightPreset *fldLightPresetTable; /* 0x00676404 */
extern FldLightFade fldLightFade;  /* 0x00677310 */

#endif /* FIELD_N19_H */
