/*
 * Sony EE kernel types (SCE eekernel.h): the thread and semaphore parameter
 * blocks of CreateThread / ReferThreadStatus and CreateSema / ReferSemaStatus.
 */
#ifndef SDK_EEKERNEL_H
#define SDK_EEKERNEL_H

#include "common.h"

#ifndef EE_THREAD_PARAM_DEFINED
#define EE_THREAD_PARAM_DEFINED
typedef struct ThreadParam {
    /* 0x00 */ s32 status;
    /* 0x04 */ void *entry;
    /* 0x08 */ void *stack;
    /* 0x0C */ s32 stackSize;
    /* 0x10 */ void *gpReg;
    /* 0x14 */ s32 initPriority;
    /* 0x18 */ s32 currentPriority;
    /* 0x1C */ u32 attr;
    /* 0x20 */ u32 option;
    /* 0x24 */ s32 waitType;
    /* 0x28 */ s32 waitId;
    /* 0x2C */ s32 wakeupCount;
} ThreadParam; /* size 0x30 */
#endif

typedef struct SemaParam {
    /* 0x00 */ s32 currentCount;
    /* 0x04 */ s32 maxCount;
    /* 0x08 */ s32 initCount;
    /* 0x0C */ s32 numWaitThreads;
    /* 0x10 */ u32 attr;
    /* 0x14 */ u32 option;
} SemaParam; /* size 0x18 */

#endif
