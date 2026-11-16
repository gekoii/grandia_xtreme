/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "battle/misc.h"
PtclBtlDust *PtclBtlDustAlloc(void) {
    s32 i; PtclBtlDust *p; PtclBtlDust *r;
 p = ptclBtlDustPool; for (i = 0; i < 128; i++, p++) { if (p->unk0 <= 0.0f) break; } r = p; if (i >= 128) r = NULL; return r; }
