/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertNotesUpdate(void) {
    u32 i;
    MiniConcertNote *e;
    f32 r;
    f32 a;
    MiniConcertRules *p;
    s32 c;
    s32 hi;
    s32 lo;
    /* Copies of e that gcse folds away; they only grow gcse's hash table
       (125 buckets), which puts miniConcertData's %hi in s7 ahead of miniConcertFx's. */
    MiniConcertNote *e0, *e1, *e2, *e3, *e4, *e5, *e6, *e7, *e8;

    for (i = 0; i < 16; i++) {
        e = &miniConcertFx->notes[i];
        e0 = e;
        e1 = e;
        e2 = e;
        e3 = e;
        e4 = e;
        e5 = e;
        e6 = e;
        e7 = e;
        e8 = e;
        if (e->unk74 == 0) {
            continue;
        }
        a = 1.0f;
        r = e->unk70;
        r = r * miniConcertWork->unk4;
        e->unk60 = r * cosf(-2.5f) + 520.0f;
        r = r * sinf(-2.5f);
        r = r + 310.0f;
        r = r + sinf(e->unk70 / miniConcertWork->unk8) * miniConcertWork->unkC;
        e->unk64 = r;
        switch (e->unk74) {
        case 0:
            continue;
        case 1:
            if (e->unk75 == miniConcertWork->unk1C) {
                e->unk74 = 2;
                e->unk75 = 0;
            } else {
                e->unk75++;
            }
            break;
        case 4:
            a = 1.0f - (f32)e->unk75 / (f32)miniConcertWork->unk1E;
            if (e->unk75 == miniConcertWork->unk1E) {
                e->unk70 = 0;
                e->unk74 = 0;
                e4->spr.active = 0;
                continue;
            }
            e->unk75++;
            break;
        case 2:
            break;
        default:
            continue;
        }
        p = miniConcertData->rules;
        c = miniConcertWork->unk16;
        hi = c + p->unk8;
        if (e->unk70 >= hi + p->unkC) {
            goto L0;
        }
        if (e->unk70 < p->unkA) {
            r = p->unk0 * e->unk70 / p->unkA;
        } else {
            r = p->unk4;
            lo = c - p->unk8;
            if (e->unk70 < lo) {
                r = (r - p->unk0) * (e->unk70 - (f32)p->unkA) / (f32)(lo - p->unkA) + p->unk0;
            }
            if (e->unk70 > hi) {
                r = (p->unk4 - p->unk0) * (1.0f - (e->unk70 - (f32)hi) / (f32)p->unkC) + p->unk0;
            }
        }
    J:
        if (e->unk70 == miniConcertWork->unk18 && e->unk76 == 0) {
            MiniConcertNoteMiss(e0);
            MiniConcertComboBreak();
            e5->unk76 = 2;
            e->unk78 += 4;
            if (e->unk73 == 1) {
                e->spr.fxFlags = 0;
                e->spr.moveCount = 0;
                e->spr.animNo = 0x12;
                e->spr.drawFlags = 2;
                e->spr.sx = e->unk68;
                e->spr.sy = e->unk6C;
                e->spr.moveTime = 6;
                GphAnimStateInit(&e2->spr.anim);
            }
        }
        if (e->unk70 == miniConcertWork->unk1A) {
            e->unk74 = 4;
            e->unk75 = 0;
        }
        if (e->unk77 != 0) {
            e6->unk77--;
        }
        e->unk70++;
        e->unk68 = r;
        e->unk6C = r;
        e->spr.ofsX = e->unk60;
        e7->spr.ofsY = e->unk64;
        e3->spr.a = a * 128.0f;
        MiniSpritesUpdate(e1, 1);
        if (e->spr.animNo != 0) {
            e->spr.sx = e->unk68;
            e->spr.sy = e->unk6C;
            if (e->spr.moveCount == e->spr.moveTime && e->unk76 == 1) {
                /* floats-first cast: EABI loads the arguments in declaration order (the real order is u8, f32, f32) */
                ((void (*)(f32, f32, u8))MiniConcertBurstSpawn)(e->unk60, e->unk64, e->unk79);
            }
            e8->spr.moveCount++;
        }
        continue;
    /* goto loop: r = p->unk0 sits out of line after the body and jumps back to J, as the original lays it out */
    L0:
        r = p->unk0;
        goto J;
    }
}
