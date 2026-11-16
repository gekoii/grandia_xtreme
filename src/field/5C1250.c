#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

typedef struct {
    u16 a[32];
    u16 b[32];
    u16 c[32];
} TrackSrc;


void ChrTenantTrackLoad(TrackSrc *src, s32 n) {
    s32 i;
    s32 j;
    TrackSrc *s;
    ChrTenantTrack *t;

    ChrTenantTrackSetCount(n);
    for (i = 0; i < n; i++) {
        s = &src[i];
        t = ChrTenantTrackGet(i);
        if (t == NULL) {
            break;
        }
        t->unk2 = 0;
        t->unk4 = 0;
        for (j = 0; j < 32; j++) {
            t->ops[j].op = s->a[j];
            t->ops[j].arg1 = s->b[j];
            t->ops[j].arg2 = s->c[j];
        }
    }
}
