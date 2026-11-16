/* Port-only C for MiniConcertComboMarksUpdate (0x00649130), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/mini.h"
#include "field/n24.h"

/*
 * Moves the combo marks (miniConcertFx->marks): state 1 spirals in to the
 * ring (120 frames), 2 orbits with the stage, 4 flies off and ends. The
 * asm reads unk70..unk78 with lhu (the header declares them s16), so they
 * are read as u16 here. The float->int stores are cvt.w.s (truncate) to
 * s32, then the low bits.
 */
void MiniConcertComboMarksUpdate(void) {
    u32 i;
    MiniConcertComboMark *m;
    f32 t;
    f32 ang;
    f32 rad;
    f32 step;
    f32 x;
    f32 y;
    f32 alpha;
    u16 age;

    miniConcertFx->unk9800 = 0;
    miniConcertFx->unk9801 = 0;
    for (i = 0; i < 128; i++) {
        m = &miniConcertFx->marks[i];
        switch (m->unk7C) {
        case 1:
            t = (u16)m->unk78 / 120.0f;
            ang = t + -2.5f;
            alpha = 1.0f;
            rad = m->unk68 - (m->unk68 - ((u16)m->unk7A * 0.8f + 50.0f)) * t;
            m->unk60 = rad * cosf(ang) + 560.0f;
            y = rad * sinf(ang) + 350.0f;
            y = y + sinf((u16)m->unk74 / 10.0f) * 5.0f;
            m->unk64 = y;
            age = m->unk78;
            if (age == 120) {
                m->unk7C = 2;
                m->unk6C = ang;
                m->unk68 = rad;
                m->unk6C = ang - miniConcertWork->unkA0 + (u16)m->unk7A * 0.25f;
                if (0.0f < m->unk6C) {
                    do {
                        m->unk6C -= 6.28318f;
                    } while (0.0f < m->unk6C);
                }
            }
            m->unk78 = age + 1;
            break;
        case 2:
            alpha = 1.0f;
            step = 0.0f;
            if (m->unk6C < 0.0f) {
                step = m->unk6C / -60.0f;
            }
            if (0.015f < step) {
                step = 0.015f;
            }
            m->unk6C = m->unk6C + step;
            if (0.0f < m->unk6C) {
                m->unk6C = 0.0f;
            }
            ang = miniConcertWork->unkA0 - (u16)m->unk7A * 0.25f + m->unk6C;
            m->unk60 = m->unk68 * cosf(ang) + 560.0f;
            y = m->unk68 * sinf(ang) + 350.0f;
            y = y + sinf((u16)m->unk74 / 10.0f) * 3.0f;
            m->unk64 = y;
            break;
        case 4:
            t = (u16)m->unk78;
            alpha = 1.0f - t / (f32)(u16)m->unk76;
            ang = miniConcertWork->unkA0 - (u16)m->unk7A * 0.25f + m->unk6C;
            m->unk60 = (f32)(u16)m->unk70 + (t + t) * cosf(ang);
            t = (u16)m->unk78;
            y = (f32)(u16)m->unk72 + (t + t) * sinf(ang);
            age = m->unk78;
            t = age;
            y = y + t * 0.1f * t;
            m->unk64 = y;
            if ((u16)m->unk76 < age) {
                m->unk7C = 0;
                m->unk74 = 0;
                continue;
            }
            m->unk78 = age + 1;
            break;
        default:
            continue;
        }
        x = m->unk60;
        m->unk74 = (u16)m->unk74 + 1;
        m->spr.ofsY = (s32)y;
        m->spr.ofsX = (s32)x;
        m->spr.a = (s32)(alpha * 128.0f);
        if (m->spr.animNo != 0) {
            if (m->spr.animNo == 0x10) {
                miniConcertFx->unk9800 = 1;
            } else {
                miniConcertFx->unk9801 = 1;
            }
            GphAnimSetSelect((GphAnimSet *)miniConcertLayout->images[m->spr.animNo + miniConcertData->hdr->unk15 - 1]);
            GphAnimStep(&m->spr.anim, 1.0f);
        }
    }
}
