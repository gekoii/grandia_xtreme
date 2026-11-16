/* Tests for the controls' mapping (port/src/input.c): the defaults by
   layout, putting a button on an input (and the swap), grandia.ini's
   writing and reading, and the capture's steps. grandia.ini is the one
   next to the test's own .exe: run it from a scratch directory.

   Build (Git Bash, from the repository root; CC is llvm-mingw's
   i686-w64-mingw32-clang, OUT a scratch directory):
     SDL=port/deps/SDL3-3.4.18/i686-w64-mingw32
     $CC -std=gnu11 -Wall -O1 -Iport/include -I$SDL/include \
         port/tests/input_test.c port/src/input.c -L$SDL/lib -lSDL3 \
         -o $OUT/input_test.exe
     cp $SDL/bin/SDL3.dll $OUT/
   Run: $OUT/input_test.exe */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "grx_input.h"

static int xbox;
static int failed;

int grx_buttons_xbox(void) {
    return xbox;
}

void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    putchar('\n');
}

#define CHECK(c)                                                        \
    do {                                                                \
        if (!(c)) {                                                     \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c);         \
            failed++;                                                   \
        }                                                               \
    } while (0)

static char *ini_text(void) {
    static char buf[4096];
    char path[1024];
    FILE *f;
    size_t n;
    snprintf(path, sizeof(path), "%sgrandia.ini", SDL_GetBasePath());
    f = fopen(path, "r");
    if (f == NULL) {
        return "";
    }
    n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = 0;
    fclose(f);
    return buf;
}

static void write_ini(const char *text) {
    char path[1024];
    FILE *f;
    snprintf(path, sizeof(path), "%sgrandia.ini", SDL_GetBasePath());
    f = fopen(path, "w");
    fputs(text, f);
    fclose(f);
}

int main(void) {
    int got;

    /* Defaults: circle on the east button, the Xbox layout's on A */
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CIRCLE) == SDL_GAMEPAD_BUTTON_EAST);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CROSS) == SDL_GAMEPAD_BUTTON_SOUTH);
    xbox = 1;
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CIRCLE) == SDL_GAMEPAD_BUTTON_SOUTH);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CROSS) == SDL_GAMEPAD_BUTTON_EAST);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_L2) == GRX_IN_LT);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_CIRCLE) == SDL_SCANCODE_X);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_L3) == GRX_IN_NONE);
    CHECK(grx_input_bit(GRX_IN_CIRCLE) == 0x20 && grx_input_bit(GRX_IN_RIGHT) == 0x2000);
    CHECK(strcmp(grx_input_name(GRX_IN_PAD, GRX_IN_LT), "LT") == 0);
    CHECK(strcmp(grx_input_name(GRX_IN_KEY, SDL_SCANCODE_RETURN), "Return") == 0);
    CHECK(strcmp(grx_input_name(GRX_IN_KEY, GRX_IN_NONE), "-") == 0);

    /* Circle on X: square, which was on X, gets circle's A */
    grx_input_set(GRX_IN_PAD, GRX_IN_CIRCLE, SDL_GAMEPAD_BUTTON_WEST);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CIRCLE) == SDL_GAMEPAD_BUTTON_WEST);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_SQUARE) == SDL_GAMEPAD_BUTTON_SOUTH);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CROSS) == SDL_GAMEPAD_BUTTON_EAST);
    /* A set map no longer follows the layout */
    xbox = 0;
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CROSS) == SDL_GAMEPAD_BUTTON_EAST);
    xbox = 1;
    CHECK(strstr(ini_text(), "[controller]\ncircle = x\ncross = b\nsquare = a\n") != NULL);
    CHECK(strstr(ini_text(), "lefttrigger") != NULL);
    CHECK(strstr(ini_text(), "[keyboard]") == NULL);

    /* L3 on a free key, then L3's key on circle: L3 gets circle's X */
    grx_input_set(GRX_IN_KEY, GRX_IN_L3, SDL_SCANCODE_F);
    grx_input_set(GRX_IN_KEY, GRX_IN_CIRCLE, SDL_SCANCODE_F);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_CIRCLE) == SDL_SCANCODE_F);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_L3) == SDL_SCANCODE_X);
    CHECK(strstr(ini_text(), "[keyboard]\ncircle = F\n") != NULL);

    /* Back to the defaults */
    grx_input_reset(GRX_IN_PAD);
    grx_input_reset(GRX_IN_KEY);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CIRCLE) == SDL_GAMEPAD_BUTTON_SOUTH);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_CIRCLE) == SDL_SCANCODE_X);
    CHECK(strstr(ini_text(), "[") == NULL);

    /* Reading: a section starts from the defaults; a line not understood
       is left out */
    write_ini("; comment\n[keyboard]\n  start = Space \ncross=Left Shift\nwhat = X\nup = nokey\n");
    grx_input_init();
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_START) == SDL_SCANCODE_SPACE);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_CROSS) == SDL_SCANCODE_LSHIFT);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_UP) == SDL_SCANCODE_UP);
    CHECK(grx_input_get(GRX_IN_KEY, GRX_IN_CIRCLE) == SDL_SCANCODE_X);
    CHECK(grx_input_get(GRX_IN_PAD, GRX_IN_CIRCLE) == SDL_GAMEPAD_BUTTON_SOUTH);

    /* The capture: everything let go first, then the first press; the game
       sees nothing until all is let go after */
    grx_input_capture(GRX_IN_PAD);
    CHECK(grx_input_poll(GRX_IN_NONE, SDL_GAMEPAD_BUTTON_SOUTH, 1, 0) == 1);
    CHECK(grx_input_captured() == -2);
    CHECK(grx_input_poll(GRX_IN_NONE, GRX_IN_NONE, 0, 0) == 1);
    CHECK(grx_input_poll(SDL_SCANCODE_Q, GRX_IN_NONE, 1, 0) == 1);
    CHECK(grx_input_captured() == -2);
    CHECK(grx_input_poll(GRX_IN_NONE, GRX_IN_RT, 1, 0) == 1);
    got = grx_input_captured();
    CHECK(got == GRX_IN_RT);
    CHECK(grx_input_poll(GRX_IN_NONE, GRX_IN_RT, 1, 0) == 1);
    CHECK(grx_input_poll(GRX_IN_NONE, GRX_IN_NONE, 0, 0) == 0);
    CHECK(grx_input_poll(GRX_IN_NONE, GRX_IN_NONE, 0, 0) == 0);

    /* Keys: the port's own are passed over, Escape cancels */
    grx_input_capture(GRX_IN_KEY);
    grx_input_poll(GRX_IN_NONE, GRX_IN_NONE, 0, 0);
    grx_input_poll(SDL_SCANCODE_F9, GRX_IN_NONE, 1, 0);
    CHECK(grx_input_captured() == -2);
    grx_input_poll(SDL_SCANCODE_ESCAPE, GRX_IN_NONE, 1, 1);
    CHECK(grx_input_captured() == -1);
    CHECK(grx_input_poll(GRX_IN_NONE, GRX_IN_NONE, 0, 0) == 0);

    write_ini("");
    printf(failed ? "%d failed\n" : "all passed\n", failed);
    return failed != 0;
}
