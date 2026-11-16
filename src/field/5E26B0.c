/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"

#define VZERO(d) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(d) : "memory")

typedef struct {
    u8 pad0[0x10];
    f32 pos[4];
    s32 unk20;
    s32 unk24;
} Node;


void VuVecCopy(void *, void *);
void ChrObjMove(ChrObj *);
void ChrBreathPuffSpawn(ChrObj *, s32);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void ChrTenantUpdate(void) {
    f32 buf[4] __attribute__((aligned(16)));
    ChrObj *p;
    ChrObj *o;
    u32 i;
    struct ChrTenantTrack *t;
    s32 g;
    f32 d;
    f32 r;
    f32 len;
    Node *n;

    p = ChrGetByNo(1);
    if (chrTenantList.count != 0) {
        for (i = 0; i < chrTenantList.count; i++) {
            o = &chrTenantList.objs[i];
            if (o == NULL) continue;
            if (!(o->flags & 0x1000)) continue;
            if (!(o->flags & 2)) continue;
            if (o->model == NULL) continue;
            /* one 64-bit load tests actFlags bit 0 and flags & 0x120 == 0x20 together */
            if ((*(u64 *)&o->actFlags & 0x12000000001ULL) == 0x2000000001ULL) {
                t = o->tracks[o->area][o->aiMode];
                if (t != NULL) {
                    ChrTenantAiUpdate(o, t);
                }
                o->speed = fsqrt(o->vel[0] * o->vel[0] + o->vel[2] * o->vel[2]);
            } else {
                VZERO(o->vel);
                o->speed = 0.0f;
            }
            if (EvtSystemFlagGet(0x70) == 1 && !(o->flags & 0x10)) {
                VZERO(o->vel);
                o->speed = 0.0f;
            }
            if (EvtSystemFlagGet(0x71) == 1 && (o->flags & 0x10)) {
                VZERO(o->vel);
                o->speed = 0.0f;
            }
            o->collMask = MapCollGetObjFlags();
            ChrTurnUpdate(o);
            VuVecCopy(buf, ((Node *)o->model)->pos);
            ChrObjMove(o);
            if ((o->flags & 0x10) && o->moveCmds == 0 && ChrTenantIsOutsideArea(o, ((Node *)o->model)->pos) == 1) {
                VuVecCopy(((Node *)o->model)->pos, buf);
            }
            ChrExtraPartsUpdate(o);
            ChrBreathPuffSpawn(o, 0);
            ChrBreathPuffSpawn(o, 1);
            o->leaderDist = FldVecDist(((Node *)p->model)->pos, ((Node *)o->model)->pos);
            ChrFindHeadPart(o);
            d = fsqrt(o->vel[0] * o->vel[0] + o->vel[2] * o->vel[2]);
            if (d > 0.001f) {
                if (o->flags & 0x80) {
                    r = ChrTenantTurnStep(o, o->speed);
                } else {
                    r = 0.0f;
                    o->moveDir = o->heading;
                }
            } else {
                r = ChrTenantTurnStep(o, 0.0f);
            }
            do { /* do-while(0): keeps the branch layout and allocation; goto body shares the motion block */
                if (!(o->flags & 0x40)) {
                body:
                    len = fsqrt(o->vel[0] * o->vel[0] + o->vel[2] * o->vel[2]);
                    o->speed = len;
                    g = o->actFlags;
                    if (g & 2) {
                        o->mot = 3;
                    } else if (g & 8) {
                        o->mot = 6;
                    } else if (len < 0.1f) {
                        o->mot = 0;
                    } else if (len < 11.99f) {
                        o->mot = 1;
                    } else {
                        o->mot = 2;
                    }
                } else if (!(o->flags & 0x100)) {
                    goto body;
                } else if (o->actFlags & 2) {
                    o->mot = 5;
                    ChrMotSet(o, o->mot);
                } else {
                    if (r > 1.0f) {
                        o->mot = 1;
                    } else {
                        o->mot = 4;
                    }
                    ChrMotSet(o, o->mot);
                }
            } while (0);
            if (!(o->flags & 0x10) || o->aiMode == 0) {
                ((Node *)o->model)->unk24 = o->heading + 0x8000;
            } else if (d > 0.001f) {
                n = (Node *)o->model;
                n->unk24 = o->heading + 0x8000;
                o->unk14++;
                if (o->unk14 >= 20) {
                    o->unk14 = 0;
                    PtclDustSpawn(n->pos, 0);
                }
            }
            if (!(o->flags & 0x40)) {
                ChrMotSet(o, o->mot);
            }
        }
        if (EvtSystemFlagGet(0x64) == 0) {
            for (i = 0; i < chrTenantList.count; i++) {
                o = &chrTenantList.objs[i];
                if (!(o->flags & 0x10)) continue;
                if (!(o->flags & 1)) continue;
                if (!(o->flags & 2)) continue;
                if (!(o->flags & 0x1000)) continue;
                if (o->actFlags & 0x400) continue;
                if (!(p->flags & 1)) continue;
                if (!(p->flags & 2)) continue;
                if (p->blinkTimer != 0) continue;
                if (o->leaderDist < o->unk38 * fldState.encountRange) {
                    fldState.unk2D = i + 1;
                }
            }
        }
    }
}
