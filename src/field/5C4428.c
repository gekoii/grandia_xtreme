/* cflags: -fno-schedule-insns */
/* -fno-schedule-insns: with scheduling on, the tail after the call uses other registers (6 words) */
#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

/* The original returns 0 in $v0 and sub->flagId in $v1: a two-register value.
   buf only sizes the frame (48 bytes, not 32); the s32-returning cast keeps the
   tail's registers as the original has them (6 words); the do-while(0) orders
   the tail (3 words). */
typedef __complex__ long Ret;

Ret ChrTenantOpHide(s32 arg0, ChrObj *obj) {
    Ret r;
    s32 buf[4];

    ((s32 (*)(void *, s32))EffSpawnChara)(obj->model->pos, 0);
    do {
        obj->flags &= 0xFFFC;
        __real__ r = 0;
        __imag__ r = CHR_SENSE(obj)->flagId;
        return r;
    } while (0);
}
