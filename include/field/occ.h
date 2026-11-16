#ifndef FIELD_OCC_H
#define FIELD_OCC_H

#include "common.h"

/*
 * FIELD "OCC" object-control triggers (OccUpdate 0x005B4128, table occTable)
 * and their "OCCS" variant (OccsUpdate 0x005B5E68, table occsTable): the
 * entries, their wake conditions and the per-kind parameters the
 * Occ<Kind>* handlers (0x005B67E0-0x005C0250) and the auto-map generator's
 * AmapOccSet* setters work on. Included by field/event.h and field/n13.h.
 */

/* ---------------------------------------------------------------- states, flags, ids */

/* OCC states (names at 0x00666AD0) */
#define OCC_SLEEP         0
#define OCC_WAKE          1
#define OCC_FIRST_WAIT    2
#define OCC_BEGIN_WAIT    3
#define OCC_FORWARD       4
#define OCC_BACKWARD_WAIT 5
#define OCC_BACKWARD      6
#define OCC_END           7

/* OccEntry.flags; 0x100 and up are per-kind bits (listed with the kinds below:
 * e.g. 0x400 is OCC_REVERSE_END for the tweens and OCC_CHR_TURN for kind 20) */
#define OCC_LOOP      0x1
#define OCC_ACTIVE    0x10
#define OCC_STOP_LOOP 0x20
#define OCC_PART      0x100
#define OCC_REVERSE_END 0x400  /* OccInterpVec: forward ends at the start value */

/* OCC / OCCS ids (s16; the debug pages show the three parts, 0x005DAB98 and
 * 0x005DB1D8): category, group and index. */
#define OCC_ID_CAT(id) (((u16)(id) >> 13) & 3)
#define OCC_ID_GRP(id) (((u16)(id) >> 8) & 0x1F)
#define OCC_ID_IDX(id) ((u8)(id))

/* Wake condition (OccCondCheck 0x005B6398): op 0 always, 1 term 0, 2 both,
 * 3 either, ...; each term tests a scenario flag/counter (EvtFlagGetByType). */
typedef struct OccCondTerm {
    /* 0x0 */ s16 type;
    /* 0x2 */ s16 id;
    /* 0x4 */ s16 value;           /* 0: the flag must be clear */
} OccCondTerm; /* size 0x6 */

typedef struct OccCond {
    /* 0x0 */ u16 op;
    /* 0x2 */ OccCondTerm terms[2];
} OccCond; /* size 0xE */

/* ---------------------------------------------------------------- per-kind parameters */

/*
 * Each OCC entry keeps its kind's parameters from +0x20 on (OccEntry.params):
 * OccConvertRecords (0x005B4E28) fills them from the map's 0x5C-byte records
 * and the Occ<Kind>* handlers use them as their work area (OccUpdate writes
 * the trap kind's hit result at +0x30). Offsets below are relative to +0x20.
 */

/* Kind 1: jump to another map (MapJumpRequest) after a fade. */
typedef struct OccMapJump {
    /* 0x00 */ s16 map;
    /* 0x02 */ s16 entrance;
    /* 0x04 */ s16 fade;           /* 0 none, 1 black, 2 white (with OCC_PART; else black) */
    /* 0x06 */ s16 fadeTime;       /* with OCC_PART; else from the map's size */
    /* 0x08 */ s16 walkDir;        /* player walks off: 0 no, 1-8 compass steps of 1000, 9 to (x, -z) */
    /* 0x0A */ u16 unkA;
    /* 0x0C */ f32 x;
    /* 0x10 */ f32 z;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ s8 unk15;
    /* 0x16 */ s16 wait;           /* frames left before the jump */
    /* 0x18 */ s16 frame;
} OccMapJump; /* size 0x1A */

/*
 * Kinds 2 Move, 3 Rotate, 4 Scale, 6 Alpha and 23 Light: tween a value of a
 * target - position, rotation (degrees, written as 16-bit angles), scale,
 * alpha ([0] only) or a light's colour, range, intensity or direction -
 * interpolated by OccInterpVec (ease 1-4), or stepped by delta every
 * interval frames (ease 0). Kind 24 (OccUvScroll) uses the same layout
 * while it interpolates.
 */
typedef struct OccTween {
    /* 0x00 */ s16 target;         /* 0 map object (part with OCC_PART), 1 event area, 2 tenant,
                                    * 3 character by id / point light (Move) / sky (Scale),
                                    * 4 party slot, 5 treasure; Light: 0 colour, 1 range,
                                    * 2 intensity, 3 direction */
    /* 0x02 */ s16 ease;           /* 0 stepwise, else OccEase mode + 1 */
    /* 0x04 */ s16 id;
    /* 0x06 */ s16 part;           /* part id, first part when stepping */
    /* 0x08 */ s16 last;           /* last part, or object count - 1 */
    /* 0x0A */ s16 time;           /* frames (u16 in the start handlers) */
    /* 0x0C */ union {
        f32 f[3];
        s32 i[3];                  /* UV scroll speeds (AmapOccSetUvScroll) */
    } to;                          /* end value, or the step when ease is 0 */
    /* 0x18 */ u16 interval;       /* frames between steps */
    /* 0x1A */ s16 save;           /* Light: 1 save the light's values in occLightSaveSlots, 2 tween back to them */
    /* 0x1C */ f32 accel;          /* added to each non-zero step component per step */
    /* 0x20 */ s16 seForward;      /* sound played at the forward start (> 0) */
    /* 0x22 */ s16 seBackward;     /* sound played at the backward start (> 0) */
    /* 0x24 */ f32 from[3];
    /* 0x30 */ union {
        f32 f[3];
        s32 i[3];                  /* Rotate steps in 16-bit angle units */
    } delta;                       /* to - from, or the current step */
    /* 0x3C */ s16 count;          /* frame counter, step countdown (u16 in the start handlers) */
} OccTween; /* size 0x3E */

/*
 * Kind 5: switch targets off (mode 0), on (1) or blink them (2) every
 * interval frames, the interval shrinking or growing by one each blink.
 */
typedef struct OccSwitch {
    /* 0x00 */ s16 target;         /* 0 map object/part (OCC_PART; OCC_TREE: part subtree too),
                                    * 1 event area, 2 sound emitter, 3 point light, 4 OCC, 5 OCCS,
                                    * 6 tenant, 7 character by id, 8 OCC sleep, 9 OCCS sleep,
                                    * 14 near object / treasure, 18 party slot, 19 sky,
                                    * 20/21 extra parts of a character / tenant */
    /* 0x02 */ s16 mode;           /* 0 off, 1 on, 2 blink */
    /* 0x04 */ s16 id;
    /* 0x06 */ s16 part;           /* first part */
    /* 0x08 */ s16 last;           /* last part, or count - 1 */
    /* 0x0A */ u16 time;
    /* 0x0C */ s16 interval;       /* blink interval at the start (u16 in the start handler) */
    /* 0x0E */ s16 ramp;           /* 0 fixed, 1 shrinking, 2 growing back to interval */
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 cur;            /* current interval (u16 in the start handler) */
    /* 0x14 */ s16 wait;           /* frames to the next toggle */
} OccSwitch; /* size 0x16 */

/* OccEntry.flags: with OCC_PART, also apply to the part's subtree (OccPartTreeSetFlags) */
#define OCC_TREE 0x200

/*
 * Kind 7: play a motion on a map model or a character, either through the
 * motion player (OCC_MOT_PLAY) or stepped here frame by frame
 * (OccMotionForward).
 */
typedef struct OccMotion {
    /* 0x00 */ s16 target;         /* 0 map model, 1 tenant, 2 character by id, 3 party slot */
    /* 0x02 */ s16 id;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 mot;            /* motion number */
    /* 0x08 */ s16 loops;          /* 0 endless */
    /* 0x0A */ s16 blend;          /* != 0: ChrMotSet, else ChrMotSetImmediate */
    /* 0x0C */ f32 length;         /* last frame */
    /* 0x10 */ s32 model;          /* model handle (stepped mode) */
    /* 0x14 */ f32 frame;
    /* 0x18 */ s16 loop;
} OccMotion; /* size 0x1A */

/* OccEntry.flags for kind 7 */
#define OCC_MOT_PLAY    0x800  /* hand the motion to the motion player */
#define OCC_MOT_HOLD    0x100  /* hold the last frame */
#define OCC_MOT_REVERSE 0x200  /* start from the last frame */

/* Kind 8: control the motion a character or map model is playing. */
typedef struct OccMotionCtrl {
    /* 0x00 */ s16 skip;           /* != 0: do nothing */
    /* 0x02 */ s16 target;         /* 0 map model, 1 tenant, 2 character by id, 3 party slot */
    /* 0x04 */ s16 id;
    /* 0x06 */ s16 op;             /* 1/2 set/clear motion flag 8; 0, 3, 5 stop (5 applies the root move first); 4 set speed */
    /* 0x08 */ f32 value;          /* speed (op 4); < 0.1 also resets the motion state */
    /* 0x0C */ s16 unkC;
} OccMotionCtrl;

/* Kind 9: show one part of a range at a time, stepping every period frames. */
typedef struct OccPartFlip {
    /* 0x00 */ s16 id;             /* map object */
    /* 0x02 */ s16 first;          /* part ids */
    /* 0x04 */ s16 last;
    /* 0x06 */ s16 period;
    /* 0x08 */ s32 model;          /* model handle */
    /* 0x0C */ s16 wait;
    /* 0x0E */ s16 cur;            /* the part shown */
} OccPartFlip; /* size 0x10 */

/* Kind 10: move a target along a stored path (MapPathBuild), with path events. */
typedef struct OccPath {
    /* 0x00 */ s16 path;           /* index into fldPathTable */
    /* 0x02 */ s16 target;         /* 0 map object, 1 event area, 3 point light, 4 tenant,
                                    * 5 character by id, 6 party slot */
    /* 0x04 */ s16 id;
    /* 0x06 */ s16 big;            /* the path is in a big slot (MapPathBuild | 0x10000) */
    /* 0x08 */ s16 slot;
    /* 0x0A */ s16 frame;
    /* 0x0C */ s16 wait;           /* frames waited at a wait event */
} OccPath; /* size 0xE */

/* OccEntry.flags for kind 10: place the target, don't walk or turn it */
#define OCC_PATH_PLACE 0x800

/*
 * Kind 11: step two parts of a map object in opposite directions (the
 * second one lag frames later), count steps of step each.
 */
typedef struct OccPartPair {
    /* 0x00 */ s16 id;             /* map object */
    /* 0x02 */ s16 part[2];        /* part ids, 0 none */
    /* 0x06 */ s16 mode;           /* 0 rotate (16-bit angles), 1 move, 2 scale */
    /* 0x08 */ union {
        s32 i[3];
        f32 f[3];
    } step;
    /* 0x14 */ u16 lag;
    /* 0x16 */ s16 count;          /* steps (6 or 10); u16 in the start handlers */
    /* 0x18 */ u16 delay;
    /* 0x1A */ s16 se;             /* sound 0xBE9 + se * 2 (+ 1 backward) when > 0 */
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ s16 done[2];        /* steps made per part */
    /* 0x38 */ s16 wait[2];        /* frames before each part starts (u16 [0] in the start handlers) */
} OccPartPair; /* size 0x3C */

/*
 * Kind 12: trap. OccUpdate wakes it when its event area reports a hit and
 * stores the hit bits (area result >> 4) in hit; OccTrapForward damages and
 * knocks back the player.
 */
typedef struct OccTrap {
    /* 0x00 */ s16 damage;         /* 0 none, 1-3: trap damage from the map set file (+0x14/+0x10/+0xC) */
    /* 0x02 */ s16 knock;          /* 0 none, 1 slide, 2 throw (FldObjMoveVelocity mode 2/3) */
    /* 0x04 */ s16 dir;            /* 0 back from the player's heading, else dir * 45 degrees */
    /* 0x06 */ s16 size;           /* knockback strength 0-2 */
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 scale;          /* knockback scale */
    /* 0x10 */ u16 hit;            /* bit 0: knock back */
    /* 0x12 */ u16 stun;           /* 0x12D-0x13C: stun presets, else frames the player can't move */
} OccTrap; /* size 0x14 */

/* Kind 14: start a camera script. */
typedef struct OccCamScript {
    /* 0x00 */ s16 mode;           /* 0: start the script (CamScrStart) */
    /* 0x02 */ s16 script;
} OccCamScript; /* used part */

/*
 * Kind 15: shake the rotation of a map object or part: each axis swings
 * around its start angle, the swing shrinking by decay at each turn.
 */
typedef struct OccShake {
    /* 0x00 */ u8 id;              /* map object */
    /* 0x01 */ u8 part;            /* 0 the whole object */
    /* 0x02 */ u8 delay[3];        /* frames before each axis starts */
    /* 0x05 */ u8 decayMode;       /* 0 none, 1-3: decay = largest amplitude * 0.1 / 0.05 / 0.2 */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 amp[3];         /* 16-bit angle units */
    /* 0x14 */ s32 accel;          /* speed change per frame */
    /* 0x18 */ s32 base[3];        /* start angles */
    /* 0x24 */ s32 decay;
    /* 0x28 */ u8 pad28[2];
    /* 0x2A */ u8 wait[3];         /* delay countdown */
    /* 0x2D */ s8 dir[3];          /* 1, -1, 0 settled */
    /* 0x30 */ s32 speed[3];
    /* 0x3C */ s32 rot[3];         /* current angles */
    /* 0x48 */ s32 swing[3];       /* current amplitude */
} OccShake; /* size 0x54 */

/* Kind 16: play a sound effect. */
typedef struct OccSound {
    /* 0x00 */ f32 pos[3];         /* mode 1: sound position; mode 2: SndSePlay's [1], [0] values */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 se;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 mode;           /* 0 plain, 1 at pos, 2 SndSePlay */
} OccSound; /* size 0x16 */

/*
 * Kind 17: set a float of map objects (GphModel +0x6C, the w of the cull
 * sphere), for a while, or saved to and restored from occObjParamSlots.
 */
typedef struct OccObjParam {
    /* 0x00 */ s16 id;             /* map object */
    /* 0x02 */ s16 part;
    /* 0x04 */ s16 last;           /* object/part count - 1 */
    /* 0x06 */ s16 type;           /* 0 the object's value (1, 2: parts, no effect) */
    /* 0x08 */ s16 op;             /* 0 set id..id+last, 1 set for time frames, 2 save to slot and set, 3 restore from slot */
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ f32 value;
    /* 0x10 */ u16 time;
    /* 0x12 */ s16 slot;           /* occObjParamSlots */
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ f32 saved;          /* op 1 */
} OccObjParam; /* size 0x1C */

/* Kind 18: set or clear a flag of a tenant or character. */
typedef struct OccChrFlag {
    /* 0x00 */ s16 target;         /* 0 tenant, 1 character by id */
    /* 0x02 */ s16 id;
    /* 0x04 */ s16 op;             /* 1 AI mode; 2-8, 13: ChrObj.flags bits; 10, 11: actFlags bits */
    /* 0x06 */ s16 clear;          /* 0 set, else clear (inverted for op 8 and 11) */
    /* 0x08 */ u16 aiMode;         /* op 1 */
} OccChrFlag; /* size 0xA */

/* Kind 19: put the player on a ladder (ChrLadderStart) between two points. */
typedef struct OccLadder {
    /* 0x00 */ f32 end0[3];        /* the two ends, same x/z */
    /* 0x0C */ f32 end1[3];
    /* 0x18 */ f32 unk18;
} OccLadder; /* size 0x1C */

/*
 * Kind 20: move a tenant or character to pos: walk there at speed
 * (FldObjMoveTo), step there over time frames (mode 1) or place it.
 */
typedef struct OccChrMove {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 target;         /* 0 tenant, 1 character by id, 2 the party gathers to the leader, 5 party slot */
    /* 0x12 */ s16 id;
    /* 0x14 */ f32 speed;          /* > 0 walk, else place */
    /* 0x18 */ s16 yaw;            /* degrees, with OCC_CHR_TURN */
    /* 0x1A */ s16 mode;           /* 1: step by vel for time frames */
    /* 0x1C */ s16 time;
    /* 0x1E */ s16 area;           /* event area id for ChrObj.htaArea, with OCC_CHR_AREA */
    /* 0x20 */ f32 vel[3];         /* mode 1: per-frame step */
} OccChrMove; /* size 0x2C */

/* OccEntry.flags for kind 20 */
#define OCC_CHR_MOVE_FLAG1 0x100  /* FldObjMoveTo flag 1 */
#define OCC_CHR_MOVE_FLAG2 0x200  /* FldObjMoveTo flag 2 */
#define OCC_CHR_TURN       0x400  /* also set the heading to yaw */
#define OCC_CHR_AREA       0x800

/* Kind 21: fade the screen (mode 0) or the field fog colour and range (mode 1). */
typedef struct OccFogFade {
    /* 0x00 */ u8 rgb[3];          /* fade colour: b, g, r ((r << 16) | (g << 8) | b) */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 color[3];       /* fog colour */
    /* 0x0A */ s16 time;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ f32 to[4];          /* mode 0: [0] fade alpha (<= 128); mode 1: fogMax, fogMin, fogDist */
    /* 0x20 */ f32 colorDelta[3];
    /* 0x2C */ f32 delta[4];
    /* 0x3C */ f32 from[4];
    /* 0x4C */ s16 colorFrom[3];
    /* 0x52 */ s16 count;
    /* 0x54 */ s16 mode;
} OccFogFade; /* size 0x56 */

/* Kind 22: miscellaneous waits. */
typedef struct OccMisc {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u32 op;             /* 6: also wait for the camera blend */
    /* 0x08 */ u32 unk8;           /* op 6: blend time; 7, 0x11-0x13: a value or map object id */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 wait;           /* frames */
} OccMisc; /* size 0x1E */

/* Kind 24: scroll the UVs of a map object's parts (GphPart.uvScroll). */
typedef struct OccUvScroll {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 ease;           /* 0 set the speeds; 1-4 interpolate (OccTween layout) */
    /* 0x04 */ s16 id;             /* map object */
    /* 0x06 */ s16 part;           /* part range; 0 and 0: every part */
    /* 0x08 */ s16 last;
    /* 0x0A */ s16 time;
    /* 0x0C */ s32 u;              /* -> uvScroll[1]; 0 and 0 turn scrolling off */
    /* 0x10 */ s32 v;              /* -> uvScroll[3] */
} OccUvScroll; /* size 0x14 */

/* Kind 26: set a scenario flag, lock the player's input or change the light preset. */
typedef struct OccStateSet {
    /* 0x00 */ s16 op;             /* 0 set a flag by type, 1 player lock, 7 light preset */
    /* 0x02 */ s16 value;          /* op 0: the value; op 1: lock (!= 0) or unlock */
    /* 0x04 */ s16 unk4;           /* op 7: FldLightPresetChange's second argument */
    /* 0x06 */ u8 unk6;            /* op 7: the preset */
    /* 0x07 */ u8 pad7;
    /* 0x08 */ u16 type;           /* EvtFlagSetByType type (4-6) */
    /* 0x0A */ s16 id;
} OccStateSet; /* size 0xC */

/* Kind 27: play a movie (sysState.movieNo). */
typedef struct OccMovie {
    /* 0x00 */ u8 movie;           /* written as s16 by OccConvertRecords */
} OccMovie; /* used part */

/*
 * Kind 29: fade the alpha of objects to a peak and back: timeIn frames up
 * to alpha, hold frames, timeOut frames to the end alpha.
 */
typedef struct OccAlphaPulse {
    /* 0x00 */ s16 ease;           /* OccEase mode */
    /* 0x02 */ s16 target;         /* 0 map object, 1 tenant, 2 character (ChrGetByNo) */
    /* 0x04 */ s16 id;
    /* 0x06 */ s16 last;           /* object count - 1 (<= 7) */
    /* 0x08 */ s16 alpha;          /* peak (u16 in the start handler) */
    /* 0x0A */ s16 timeIn;
    /* 0x0C */ u16 alphaEnd;       /* used with OCC_PART (0x100), else each object's start alpha */
    /* 0x0E */ s16 timeOut;
    /* 0x10 */ s16 hold;
    /* 0x12 */ u16 backTime;       /* backward timer */
    /* 0x14 */ s16 start[8];       /* alpha per object at the start */
    /* 0x24 */ s16 end[8];
    /* 0x34 */ s16 phase;          /* 0 in, 1-2 hold, 3 out, 4 done */
    /* 0x36 */ s16 frame;
    /* 0x38 */ s16 delta[8];
} OccAlphaPulse; /* size 0x48 */

/*
 * Kind 30: collapse a range of parts of a map object to the object's origin
 * (scaling their positions to 0), or expand them back, with random jitter.
 */
typedef struct OccPartCollapse {
    /* 0x00 */ u8 id;              /* map object */
    /* 0x01 */ u8 first;           /* parts first..last, at most 12 */
    /* 0x02 */ u8 last;
    /* 0x03 */ u8 ease;            /* OccEase mode */
    /* 0x04 */ u8 expand;          /* 0 collapse, 1 expand */
    /* 0x05 */ u8 time;
    /* 0x06 */ u8 backTime;        /* backward timer - 1 */
    /* 0x07 */ u8 jitter[3];       /* OccRandSpread range per axis */
    /* 0x0A */ u8 padA;
    /* 0x0B */ u8 frame;
    /* 0x0C */ s16 pos[12][3];     /* the parts' positions at the start */
} OccPartCollapse; /* size 0x54 */

/* Kind 31: screen wipe (GphWipeStart). */
typedef struct OccWipe {
    /* 0x00 */ s16 param1;         /* GphWipeStart's third argument; >= 2 (unsigned) stops the wipe */
    /* 0x02 */ s8 type;            /* the converter writes s16s at +2 and +4 */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s8 param0;
} OccWipe; /* used part */

/*
 * Kind 33: play a motion on a character for its length, turning it,
 * then switch to the next motion.
 */
typedef struct OccMotionTimed {
    /* 0x00 */ s16 target;         /* 0 map object, 1 tenant, 2 character by id, 3 party slot */
    /* 0x02 */ s16 id;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 mot;
    /* 0x08 */ s16 nextMot;        /* < 0: motion 0 and release the character */
    /* 0x0A */ s16 turn;           /* degrees over the motion */
    /* 0x0C */ s16 turnEnd;        /* degrees at the end */
    /* 0x0E */ s8 blend;           /* != 0: ChrMotSet, else ChrMotSetImmediate */
    /* 0x0F */ s8 rootMove;        /* apply the root move at the end */
    /* 0x10 */ f32 speed;          /* motion speed, with OCC_MOT_REVERSE (0x200) */
    /* 0x14 */ f32 unk14;
    /* 0x18 */ s32 model;          /* model handle */
    /* 0x1C */ f32 turnStep;       /* per frame, 16-bit units / 182.04 */
    /* 0x20 */ f32 length;
    /* 0x24 */ union {
        s32 i;
        f32 f;
    } frame;
} OccMotionTimed; /* size 0x28 */

/*
 * Kind 35: set (1) or clear (2) several flags of a tenant or character;
 * 0 leaves a flag alone.
 */
typedef struct OccChrFlags {
    /* 0x00 */ s16 target;         /* 0 tenant, 1 character by id + 1, 2 party slot */
    /* 0x02 */ s16 id;
    /* 0x04 */ s8 op[11];          /* ChrObj.flags 8, 4, 0x400, 0x800, -, 0x80; tenants only:
                                    * 0x20, 0x1002 (inverted), actFlags 1 (2 also stops it),
                                    * flags 0x200, actFlags 2 (inverted) */
} OccChrFlags; /* size 0xF */

/* Kind 36: lens flare (FldLensFlareSet). */
typedef struct OccLensFlare {
    /* 0x00 */ s16 enable;
    /* 0x02 */ s16 x;              /* sun position */
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
} OccLensFlare; /* size 0x8 */

/* Kind 39: screen effect on fldScreenFx (< 8), or the shatter transition. */
typedef struct OccScreenFx {
    /* 0x00 */ u8 effect;          /* GphScreenFxSet effect (1 becomes 7, fixed blur); >= 8 shatter */
    /* 0x01 */ u8 noHold;          /* passed inverted */
    /* 0x02 */ s16 time;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
} OccScreenFx; /* size 0x10 */

/* Kind 40: spawn a map effect (EffSpawnByCode) at a part of a map object. */
typedef struct OccMapEffect {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 pad11;
    /* 0x12 */ u8 effect;          /* effect code - 1 */
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s16 id;             /* map object */
    /* 0x16 */ s16 part;           /* with OCC_PART, else part 0 */
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
} OccMapEffect; /* size 0x1C */

/* Kind 41: put the field camera at pos (camera AI mode 4). */
typedef struct OccCameraSetPos {
    /* 0x00 */ f32 pos[3];         /* copied as a vector to the camera eye */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ f32 unk1C;          /* -> camCtl.unk14 */
    /* 0x20 */ f32 fov;
    /* 0x24 */ u8 unk24;
} OccCameraSetPos; /* size 0x28 */

/* Kind 43: save a map checkpoint and re-enter the map. */
typedef struct OccMapReload {
    /* 0x00 */ u8 unk0;            /* EvtSaveMapCheckpoint's arguments */
    /* 0x01 */ u8 unk1;
} OccMapReload; /* used part */

/* Kind 44: pad vibration (PadVibrate). */
typedef struct OccVibrate {
    /* 0x00 */ u16 time;
    /* 0x02 */ u8 motor;
    /* 0x03 */ u8 level;
} OccVibrate; /* size 0x4 */

/* Kind 45: set a character's face (ChrObj.face). */
typedef struct OccChrFace {
    /* 0x00 */ u8 target;          /* 0 character by id, 1 tenant */
    /* 0x01 */ u8 noBlink;
    /* 0x02 */ u16 id;
    /* 0x04 */ u8 face;
} OccChrFace; /* used part */

/* Kind 46: turn a character's body toward another character or an angle (ChrTurnStart). */
typedef struct OccChrTurn {
    /* 0x00 */ u8 toChr;           /* != 0: toward the other character, else to angle */
    /* 0x01 */ u8 target;          /* 0 character by id, 1 tenant, 2 party slot */
    /* 0x02 */ u8 otherTarget;     /* the same for the other character */
    /* 0x03 */ u8 mode;
    /* 0x04 */ u16 id;
    /* 0x06 */ u16 otherId;
    /* 0x08 */ f32 speed;
    /* 0x0C */ f32 angle;
    /* 0x10 */ u8 unk10;
} OccChrTurn; /* used part */

/* Kind 47: turn a character's head (ChrHeadTurnStart); as kind 46 but +0x10 is a float. */
typedef struct OccChrLook {
    /* 0x00 */ u8 toChr;
    /* 0x01 */ u8 target;          /* 0 character by id, 1 tenant, 2 party slot */
    /* 0x02 */ u8 otherTarget;
    /* 0x03 */ u8 mode;            /* 2 shakes the head */
    /* 0x04 */ u16 id;
    /* 0x06 */ u16 otherId;
    /* 0x08 */ f32 speed;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 angle;
} OccChrLook; /* size 0x14 */

/* Kind 48: start or stop a field particle effect. */
typedef struct OccParticle {
    /* 0x00 */ u16 effect;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s32 start;          /* != 0 start (PtclFldEffectStart), else stop */
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ f32 pos[4];
    /* 0x20 */ f32 rot[4];
} OccParticle; /* size 0x30 */

/* Kind 50: quake - camera shake, pad vibration and blur by strength. */
typedef struct OccQuake {
    /* 0x00 */ u16 mode;           /* 0 stop, 1 quake, 2 preset, 3 vertical, 4 fixed vector,
                                    * 5/6: 3/4 with stop, vibration and blur */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 strength;       /* > 80 strong, > 40 medium vibration */
    /* 0x08 */ s32 time;
} OccQuake; /* size 0xC */

extern f32 occObjParamSlots[];         /* kind 17 save slots (OccObjParam.slot) */

/* ---------------------------------------------------------------- OCC entries */

typedef union OccParams {
    u8 raw[0x60];
    OccMapJump mapJump;                /* 1 */
    OccTween tween;                    /* 2, 3, 4, 6, 23 */
    OccSwitch sw;                      /* 5 */
    OccMotion motion;                  /* 7 */
    OccMotionCtrl motionCtrl;          /* 8 */
    OccPartFlip partFlip;              /* 9 */
    OccPath path;                      /* 10 */
    OccPartPair partPair;              /* 11 */
    OccTrap trap;                      /* 12 */
    OccCamScript camScript;            /* 14 */
    OccShake shake;                    /* 15 */
    OccSound sound;                    /* 16 */
    OccObjParam objParam;              /* 17 */
    OccChrFlag chrFlag;                /* 18 */
    OccLadder ladder;                  /* 19 */
    OccChrMove chrMove;                /* 20 */
    OccFogFade fogFade;                /* 21 */
    OccMisc misc;                      /* 22 */
    OccUvScroll uvScroll;              /* 24 */
    OccStateSet stateSet;              /* 26 */
    OccMovie movie;                    /* 27 */
    OccAlphaPulse alphaPulse;          /* 29 */
    OccPartCollapse partCollapse;      /* 30 */
    OccWipe wipe;                      /* 31 */
    OccMotionTimed motionTimed;        /* 33 */
    OccChrFlags chrFlags;              /* 35 */
    OccLensFlare lensFlare;            /* 36 */
    OccScreenFx screenFx;              /* 39 */
    OccMapEffect mapEffect;            /* 40 */
    OccCameraSetPos cameraSetPos;      /* 41 */
    OccMapReload mapReload;            /* 43 */
    OccVibrate vibrate;                /* 44 */
    OccChrFace chrFace;                /* 45 */
    OccChrTurn chrTurn;                /* 46 */
    OccChrLook chrLook;                /* 47 */
    OccParticle particle;              /* 48 */
    OccQuake quake;                    /* 50 */
} OccParams; /* size 0x60 */

typedef struct OccEntry {
    /* 0x00 */ s16 timer;
    /* 0x02 */ s16 state;          /* OCC_* */
    /* 0x04 */ s16 id;             /* OccFindById; OCC_ID_* parts */
    /* 0x06 */ s16 unk6;
    /* 0x08 */ OccCond cond;
    /* 0x16 */ u16 flags;          /* OCC_LOOP ... */
    /* 0x18 */ s16 kind;           /* handler tables index, parameter layout */
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ OccParams params;
} OccEntry; /* size 0x80 */

/* cond.terms[1] and flags (+0x10-+0x17) as one doubleword: OccUpdate tests
 * flags & 0x82 with one ld. */
#define OCC_TERM1_FLAGS(e) (*(u64 *)&(e)->cond.terms[1])

/* The handlers reach the parameters through this cast as often as through
 * e->params (same offsets). */
#define OCC_PARAMS(e, type) ((type *)((u8 *)(e) + 0x20))

/* Per-kind OCC handler (occBeginTable, occEndTable, occForwardStartTable,
 * occForwardTable, occBackwardStartTable, occBackwardTable; NULL = none). */
typedef void (*OccHandler)(struct OccEntry *);

typedef struct OccTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ OccEntry *items;
} OccTable;

/* ---------------------------------------------------------------- OCCS */

typedef struct OccsCmd {
    /* 0x0 */ s16 op;
    /* 0x2 */ s16 arg;
} OccsCmd;

typedef struct OccsEntry {
    /* 0x00 */ s8 state;           /* read as s16 with +1 in 0x005B5E08/0x005B6338 */
    /* 0x01 */ s8 idx;
    /* 0x02 */ s16 id;             /* OccsFindById; stored as u16 by 0x005B66D0 */
    /* 0x04 */ s16 timer;
    /* 0x06 */ u16 flags;
    /* 0x08 */ s16 areaId;         /* event area (EvtAreaGet) */
    /* 0x0A */ OccCond cond;
    /* 0x18 */ OccsCmd cmds[32];
} OccsEntry; /* size 0x98 */

typedef struct OccsTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ OccsEntry *items;
} OccsTable;

#endif /* FIELD_OCC_H */
