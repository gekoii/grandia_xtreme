/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n28.h"

void MenuAutoCamPanelUpdate(void) {
    MenuAutoCamPanel *m = &menuAutoCamPanel;
    f32 t;
    f32 v;
    f32 cur;
    if (m->state == 1) {
        if (m->alpha < 1.0f) {
            m->alpha += 0.1f;
            if (m->alpha > 1.0f) {
                m->alpha = 1.0f;
            }
        }
        if (m->slide > 0.0f) {
            if (m->slideVel > 0.0f) {
                m->slideVel = 0.0f;
            }
            m->slideVel -= 1.0f;
            m->slide += m->slideVel;
            if (m->slide < 0.0f) {
                m->slideVel = 0.0f;
                m->slide = 0.0f;
            }
        }
    } else if (m->alpha > 0.0f) {
        m->alpha -= 0.1f;
        if (m->alpha <= 0.0f) {
            m->state = 0;
            m->slide = 64.0f;
            m->alpha = 0.0f;
            m->slideVel = 0.0f;
            m->mode = 0;
            m->cursorVel = 0.0f;
            m->cursorX = 0.0f;
        } else {
            if (m->slideVel < 0.0f) {
                m->slideVel = 0.0f;
            }
            m->slideVel += 1.0f;
            m->slide += m->slideVel;
        }
    }
    cur = m->cursorX;
    t = m->mode * 32.0f;
    if (cur < t) {
        m->cursorVel += 2.0f;
        v = cur + m->cursorVel;
        m->cursorX = v;
        if (t <= v) {
            m->cursorVel = 0.0f;
            m->cursorX = t;
            cur = t;
        } else {
            cur = v;
        }
    } else if (t < cur) {
        m->cursorVel -= 2.0f;
        v = cur + m->cursorVel;
        m->cursorX = v;
        if (v <= t) {
            m->cursorX = t;
            cur = t;
            m->cursorVel = 0.0f;
        } else {
            cur = v;
        }
    }
    if (cur < 0.0f) {
        m->cursorX = 0.0f;
    } else if (cur > 96.0f) {
        m->cursorX = 96.0f;
    }
}
