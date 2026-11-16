#ifndef PAD_H
#define PAD_H

/*
 * Controllers. PadUpdate (0x0042D388) reads both ports every frame into the
 * two-entry pad array at 0x004F2520 (padPort0, padPort1 = padPort0 + 0x54);
 * PadUpdateRepeat recomputes the repeat bits on skipped frames.
 *
 * Button bits (after PadReadPort inverts libpad's active-low word):
 *   0x0001 L2, 0x0002 R2, 0x0004 L1, 0x0008 R1, 0x0010 triangle,
 *   0x0020 circle, 0x0040 cross, 0x0080 square, 0x0100 select,
 *   0x0800 start, 0x1000 up, 0x2000 right, 0x4000 down, 0x8000 left.
 * The left stick also sets the d-pad bits past 0x30/0xD0.
 */

#include "common.h"

typedef struct Pad {
    /* 0x00 */ u16 held;         /* buttons down (named hold/flags/btn/trg/unk0 in views) */
    /* 0x02 */ u16 released;     /* went up this frame */
    /* 0x04 */ u16 trigger;      /* went down this frame (padPort0Trigger, padTrigger) */
    /* 0x06 */ u16 repeat;       /* trigger, then every 9th frame after 15 held (menus' "rep") */
    /* 0x08 */ u16 repeatFast;   /* trigger, then every frame after 15 held */
    /* 0x0A */ u16 prevHeld;
    /* 0x0C */ u8 modeId;        /* 4 digital, 7 analog, 0 none ("ID" on the PAD page) */
    /* 0x0D */ u8 pressureMode;  /* pressure data is read (set once the mode request succeeds) */
    /* 0x0E */ u8 pressure[16];  /* button pressure, zeroed when pressureMode is 0; PadReadPort fills
                                  * 12, FldPadApplyInputLock (5C95F0) clears 16 */
    /* 0x1E */ u8 rightX;        /* sticks, 0x80 centre, inverted from libpad */
    /* 0x1F */ u8 rightY;
    /* 0x20 */ u8 leftX;         /* > 0xD0 sets left, < 0x30 right */
    /* 0x21 */ u8 leftY;         /* > 0xD0 sets up, < 0x30 down */
    /* 0x22 */ u8 pad22;
    /* 0x23 */ u8 vibActive;     /* PadVibrate sets it, PadUpdate runs padVibState while set */
    /* 0x24 */ u8 repeatCount[16];     /* per button frame counters of .repeat */
    /* 0x34 */ u8 repeatStarted[16];   /* past the initial 15-frame delay */
    /* 0x44 */ u8 repeatFastCount[16]; /* per button frame counters of .repeatFast */
} Pad; /* size 0x54 */

/*
 * The pad array at 0x004F2520 has two entries; each also has its own
 * symbol. padPort0 is declared as the whole array: SysMain reads pad 1 as
 * padPort0[1], and only matches when the declared object covers it (gcc then
 * reuses padPort0's address register with offset 0x58 instead of a new
 * %hi/%lo pair for padPort1).
 */
extern Pad padPort0[2];
extern Pad padPort1;
extern u16 padPort0Trigger;     /* padPort0.trigger */
extern u16 padTrigger;          /* padPort1.trigger */

/* Pad the debug pages read (= &padPort1). */
extern Pad *dbgPad;

/* PadVibrate request, played by PadUpdate. */
typedef struct PadVibState {
    /* 0x0 */ u8 active;
    /* 0x1 */ u8 time;     /* frames left; u8 decided by PadUpdate's lbu (s8 in PadVibrate, which only stores it) */
    /* 0x2 */ u8 small;    /* small motor on/off */
    /* 0x3 */ u8 big;      /* big motor level */
    /* 0x4 */ u8 decay;    /* subtracted from big each frame */
} PadVibState;

extern PadVibState padVibState;
extern s32 padType[2];      /* terminal type nibble per port (PadReadPort) */
extern s32 padLastType[2];
extern s32 padPhase[2];     /* mode setup state per port */
extern s32 padModeId[2];    /* 4 digital, 7 analog */
extern s32 padExtModeId[2];
extern u8 padActDirect[6];  /* scePadSetActDirect values (small, big) */
extern u8 padActAlign[6];   /* scePadSetActAlign */
extern s32 padVibEnabled;
extern s32 padInputLocked;
extern u8 dbgPadMotorLeft;
extern u8 dbgPadMotorRight;

#endif
