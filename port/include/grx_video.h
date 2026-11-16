/* The frame loop and video output (port/src/video.c): an SDL3 window with
   an OpenGL 3.3 core context that shows what the GS displays, and the NTSC
   field clock that drives the kernel's vblank.

   Two clocks. Fields come from the kernel's idle hook: when every game
   thread waits, the frame loop publishes the picture the GS displays,
   waits in real time for the next field (59.94 Hz), then runs it
   (grx_gs_vsync, grx_kernel_field, the pad update) and wakes the threads
   waiting for a vblank. The game's vsync waits (GsVsyncFlip's loop on
   gsFlipWaitCount, _sceWaitVSync, _sceWaitVSyncCsr) block in
   grx_frame_wait_vblank() instead of polling.

   Presents come from the presenter thread, which owns the GL context: at
   the display's refresh rate (vsync), each shows the newest picture the
   game thread published, so a 120 Hz display gets 120 presents a second
   while the game's fields stay at 59.94. A present hook (below) can give
   each present its own picture, for frame interpolation. GRX_VIDEO_THREAD=0
   presents on the game thread instead, once per field, without vsync.

   Two sources of pictures. By default the frame loop reads the software
   GS's picture (grx_gs_display) at each field and uploads it. A GPU
   renderer instead draws into GL textures of its own, in a context that
   shares them with the presenter's, and submits them (grx_video_gl_*).

   Unless noted, call these on the OS thread that runs the game, like
   grx_kernel.h. No game names, so it can go next to <windows.h>. */
#ifndef GRX_VIDEO_H
#define GRX_VIDEO_H

#include <stdint.h>

typedef struct GrxVideoConfig {
    int scale;       /* window size: 640x480 times this (default 2) */
    int fullscreen;  /* start fullscreen (Alt+Enter / F11 toggle) */
    int integer;     /* integer vertical scale factors only */
    int smooth;      /* 1 linear filtering, 0 nearest, -1 the default (nearest with integer, else linear) */
    int headless;    /* no window, no GL: fields are still paced, pads still read */
    int hidden_gl;   /* with headless: a hidden window all the same, for a GPU renderer's
                        GL context (nothing is presented) */
    int log_fields;  /* log the field and present rates once a second */
    double seconds;  /* > 0: exit(0) after this much real time (tests) */
    int test_pattern;/* draw a moving test picture when the GS shows nothing */
} GrxVideoConfig;

/* SDL_Init (video, gamepad, events), the window, its GL context, the pads.
   Call after grx_hwmem_init(). Returns 0, or -1 (logged) if there is no
   window or no GL 3.3; the frame loop then runs headless. The context
   stays current on this thread until grx_frame_start() hands it to the
   presenter thread. */
int grx_video_init(const GrxVideoConfig *cfg);
void grx_video_shutdown(void);

/* Publish what the GS displays now (grx_gs_display, or the test picture)
   as the picture to present. The frame loop calls it once per field. */
void grx_video_present(void);

/* Handle window and input events: window close exits (exit(0)) */
void grx_video_pump(void);

/* Install the kernel idle hook that drives the fields, and start the
   presenter thread */
void grx_frame_start(void);

/* From a game thread: block until the next field's vblank has been run.
   From an interrupt handler or the idle loop (which can't block) it runs
   a field at once instead. */
void grx_frame_wait_vblank(void);

/* For loops that spin on hardware state the platform layer updates
   (DMA, SIF): if a field is due in real time, run it now. Keeps vblank
   handlers, the window and the pads alive while no thread waits. */
void grx_frame_poll(void);

/* For the game's empty counting loops (for (i = 0; i < n; i++) {}),
   which compile to nothing here: n passes take as long as on the EE,
   about 3 cycles each, with fields run as they come due. Limits that
   count passes of a loop with one in it then last as long as they did. */
void grx_ee_spin(uint32_t loops);

/* Fields run since grx_frame_start */
uint64_t grx_frame_count(void);

/* ---------------------------------------------------------------------- */
/* Pictures as GL textures: the GPU renderer and the present hook */

typedef struct GrxVideoFrame {
    unsigned texture;  /* a GL_TEXTURE_2D in the presenter's share group; 0: no picture (black) */
    int width, height; /* its size in texels (any size) */
    float aspect;      /* the picture's display aspect, width over height (4/3 for the GS's) */
    int top_down;      /* 1: texel row 0 is the picture's top line (CPU order, as uploaded);
                          0: GL's bottom-up order, as rendering into an FBO gives */
    /* Set by the video layer when the picture is published or submitted: */
    uint64_t field;    /* the field it was published at (grx_frame_count) */
    uint64_t time_ns;  /* when that field was due, on SDL_GetTicksNS's clock: pictures
                          60 a second are 1001/60 ms apart here, whatever the jitter */
} GrxVideoFrame;

/* 1 if presenting through GL (a window, not headless) */
int grx_video_gl_available(void);

/* The window's size in pixels now (0 x 0 without a window); any thread */
void grx_video_window_size(int *w, int *h);

/* A new GL 3.3 core context that shares textures, buffers and sync
   objects with the presenter's: for the GPU renderer. Only before
   grx_frame_start() (grx_gs_init is the place). It is current on the
   calling thread when this returns; to use it on another thread, call
   grx_video_gl_make_current(NULL) here, then grx_video_gl_make_current(ctx)
   there. Draw into FBOs only, never the window's framebuffer. NULL (logged)
   without GL. The video layer destroys it at shutdown. */
void *grx_video_gl_create_context(void);
int grx_video_gl_make_current(void *ctx); /* 0, or -1 (logged); any thread */

/* Choose the pictures to present: 1 the GL textures grx_video_gl_submit()
   gives, 0 the software GS's (the default). Any thread. */
void grx_video_gl_source(int on);

/* From the renderer's thread, with its shared context current, once its
   commands for the picture are issued: present this texture from now on.
   The video layer fences the commands, so the presenter never shows a
   half-drawn picture, and stamps field and time_ns. */
void grx_video_gl_submit(const GrxVideoFrame *frame);

/* From the renderer's thread, before drawing into a texture it submitted
   before: 1 if the presenter is done with it (the renderer's context then
   waits, on the GPU, for the presenter's last use), 0 if still in use.
   The presenter holds at most three submitted textures (the previous and
   current pictures, and one not yet presented), so a renderer that
   rotates through four always finds one free. Never blocks. */
int grx_video_gl_texture_free(unsigned texture);

/* The present hook, for frame interpolation: called on the presenting
   thread, with the presenter's context current, once per present (so at
   the display's refresh rate), before it draws. prev and cur are the two
   newest pictures (prev->texture is 0 until there are two; both are
   0-texture when there is no picture). t is where this present falls
   between them if the display runs one picture behind the game:
   (now - cur->time_ns) / (cur->time_ns - prev->time_ns), clamped to 0..1
   (0: prev's moment, 1: cur's). now_ns is SDL_GetTicksNS() at the call.
   Return 1 with *out set (e.g. a texture the hook rendered for time t, in
   its own FBO) to present that instead, or 0 to present cur. The hook may
   change any GL state: the presenter sets what it uses afterwards. With
   the software GS a picture is published every field, so at 30 game
   frames a second consecutive pictures can be the same. */
typedef int (*GrxVideoPresentHook)(void *user, const GrxVideoFrame *prev, const GrxVideoFrame *cur, double t,
                                   uint64_t now_ns, GrxVideoFrame *out);
void grx_video_set_present_hook(GrxVideoPresentHook hook, void *user); /* any thread; NULL removes it */

#endif
