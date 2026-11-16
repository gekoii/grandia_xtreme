/* Checks of the presenter's GL interfaces for --video-test (grx_gl.h), each
   through grx_video.h alone, the way the GPU renderer and the frame
   interpolation will use them:

   GRX_VIDEO_GL_TEST=1: a stand-in GPU renderer. A shared context on the
   game thread draws the test picture at 1440x1080 into a rotation of four
   textures (FBO, scissored clears) and submits one per game frame.

   GRX_VIDEO_HOOK_TEST=1: a stand-in interpolation stage. The present hook
   cross-fades the two newest pictures by t into a texture of its own, and
   logs the t it was given every few seconds. */
#include <stdint.h>
#include <SDL3/SDL.h>
#include "grx_gl.h"
#include "grx_video.h"
#include "grx_platform.h"

#define TW 1440
#define TH 1080
#define NTEX 4

static GLuint tex[NTEX], fbo;
static int next_tex;

int grx_gl_test_init(void) {
    int i;
    if (grx_video_gl_create_context() == NULL) {
        return -1;
    }
    /* Current on this (the game's) thread from here on */
    glGenTextures(NTEX, tex);
    for (i = 0; i < NTEX; i++) {
        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TW, TH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    glGenFramebuffers(1, &fbo);
    grx_video_gl_source(1);
    grx_log("gl-test: the test picture through the GPU renderer interface, %dx%d, %d textures", TW, TH, NTEX);
    return 0;
}

/* A rectangle in top-down picture coordinates, filled */
static void rect(int x, int y, int w, int h, uint32_t rgb) {
    glScissor(x, TH - y - h, w, h);
    glClearColor((float)((rgb >> 16) & 255) / 255.0f, (float)((rgb >> 8) & 255) / 255.0f,
                 (float)(rgb & 255) / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void grx_gl_test_frame(uint64_t field) {
    static const uint32_t bars[8] = {0xC0C0C0, 0xC0C000, 0x00C0C0, 0x00C000, 0xC000C0, 0xC00000, 0x0000C0, 0x000000};
    static int warned;
    GrxVideoFrame f;
    int i, k = -1, bx;

    for (i = 0; i < NTEX && k < 0; i++) {
        if (grx_video_gl_texture_free(tex[(next_tex + i) % NTEX])) {
            k = (next_tex + i) % NTEX;
        }
    }
    if (k < 0) {
        if (!warned) {
            warned = 1;
            grx_log("gl-test: no texture free (the presenter holds at most three)");
        }
        return;
    }
    next_tex = k + 1;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[k], 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        grx_fatal("gl-test: framebuffer incomplete");
    }
    glViewport(0, 0, TW, TH);
    glEnable(GL_SCISSOR_TEST);
    for (i = 0; i < 8; i++) {
        rect(i * TW / 8, 0, TW / 8, TH * 320 / 448, bars[i]);
    }
    rect(0, TH * 320 / 448, TW, TH - TH * 320 / 448, 0x303030);
    bx = (int)(field % 600) * TW / 600;
    rect(bx, TH * 340 / 448, TW / 16, TH * 80 / 448, 0xFFFFFF);
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    f.texture = tex[k];
    f.width = TW;
    f.height = TH;
    f.aspect = 4.0f / 3.0f;
    f.top_down = 0;
    f.field = 0;
    f.time_ns = 0;
    grx_video_gl_submit(&f);
}

/* ---------------------------------------------------------------------- */
/* The hook: on the presenter's thread, in its context */

static const char hook_vs[] = "#version 330 core\n"
                              "out vec2 p;\n"
                              "void main() {\n"
                              "    p = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));\n"
                              "    gl_Position = vec4(p * 2.0 - 1.0, 0.0, 1.0);\n"
                              "}\n";
static const char hook_fs[] = "#version 330 core\n"
                              "uniform sampler2D a, b;\n"
                              "uniform vec4 flip_t;\n" /* a top-down, b top-down, t */
                              "in vec2 p;\n"
                              "out vec4 color;\n"
                              "void main() {\n"
                              "    vec2 pa = vec2(p.x, flip_t.x > 0.5 ? 1.0 - p.y : p.y);\n"
                              "    vec2 pb = vec2(p.x, flip_t.y > 0.5 ? 1.0 - p.y : p.y);\n"
                              "    color = mix(texture(a, pa), texture(b, pb), flip_t.z);\n"
                              "}\n";

static GLuint hprog, hvao, hfbo, htex;
static GLint hu_flip_t;
static int hw, hh, hfailed;
static uint64_t hcalls, hlog_ns;
static double tmin = 2, tmax = -1, tsum;

static int hook(void *user, const GrxVideoFrame *prev, const GrxVideoFrame *cur, double t, uint64_t now_ns,
                GrxVideoFrame *out) {
    (void)user;
    hcalls++;
    tsum += t;
    tmin = t < tmin ? t : tmin;
    tmax = t > tmax ? t : tmax;
    if (hlog_ns == 0) {
        hlog_ns = now_ns;
    } else if (now_ns - hlog_ns >= 2000000000ull) {
        grx_log("hook-test: %llu presents, t %.3f..%.3f, mean %.3f", (unsigned long long)hcalls, tmin, tmax,
                tsum / (double)hcalls);
        hlog_ns = now_ns;
        hcalls = 0;
        tsum = 0;
        tmin = 2;
        tmax = -1;
    }
    if (hfailed || prev->texture == 0 || cur->texture == 0) {
        return 0;
    }
    if (hprog == 0) {
        hprog = grx_gl_program(hook_vs, hook_fs, "hook-test");
        if (hprog == 0) {
            hfailed = 1;
            return 0;
        }
        glUseProgram(hprog);
        glUniform1i(glGetUniformLocation(hprog, "a"), 0);
        glUniform1i(glGetUniformLocation(hprog, "b"), 1);
        hu_flip_t = glGetUniformLocation(hprog, "flip_t");
        glGenVertexArrays(1, &hvao);
        glGenFramebuffers(1, &hfbo);
        glGenTextures(1, &htex);
    }
    if (hw != cur->width || hh != cur->height) {
        hw = cur->width;
        hh = cur->height;
        glBindTexture(GL_TEXTURE_2D, htex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, hw, hh, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, hfbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, htex, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            grx_log("hook-test: framebuffer incomplete");
            hfailed = 1;
            return 0;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, hfbo);
    glViewport(0, 0, hw, hh);
    glUseProgram(hprog);
    glBindVertexArray(hvao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, prev->texture);
    glBindSampler(0, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, cur->texture);
    glBindSampler(1, 0);
    glUniform4f(hu_flip_t, (float)prev->top_down, (float)cur->top_down, (float)t, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glActiveTexture(GL_TEXTURE0);
    *out = *cur;
    out->texture = htex;
    out->top_down = 0;
    return 1;
}

void grx_gl_test_hook_install(void) {
    grx_video_set_present_hook(hook, NULL);
    grx_log("hook-test: cross-fading the two newest pictures by t at every present");
}
