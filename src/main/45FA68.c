#include "common.h"
#include "snd.h"
typedef struct { s32 a; s32 b; s32 c; s32 d; s32 e; } Dst;
s32 SndIopGetDriveStat(Dst *d) {
    if (d != NULL) {
    d->b = sndIopStatus->driveUnk10; d->c = sndIopStatus->driveUnk1C; d->d = 0; d->e = sndIopStatus->driveUnk28; d->a = sndIopStatus->driveStat;
    }
    return sndIopStatus->driveStat;
}
