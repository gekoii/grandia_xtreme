/* The Xbox button art (port/src/buttons.c) on a texture taken from the
   game: writes it before and after, as PPM, to look at.

   Get a texture's 64 KB of pixels and 1 KB of palette from a frozen game
   (port/docs/debug.md; port/docs/xbox.md says where they are), then build
   (from the repository root):
     i686-w64-mingw32-clang -std=gnu11 -Wall -O1 -DGRX_BUTTONS_ART_ONLY -Iport/include \
         port/tests/buttons_art.c port/src/buttons.c -o buttons_art.exe
   Run: buttons_art.exe icons|hud PIXELS CLUT OUT_PREFIX */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "grx_buttons.h"

static uint8_t px[65536];
static uint32_t clut[256];

static int slot(int i) {
    int b = i & 0x18;
    return (i & ~0x18) | (b == 8 ? 16 : b == 16 ? 8 : b);
}

static void write_ppm(const char *path) {
    FILE *f = fopen(path, "wb");
    int i;
    if (f == NULL) {
        return;
    }
    fprintf(f, "P6\n256 256\n255\n");
    for (i = 0; i < 65536; i++) {
        uint32_t c = clut[slot(px[i])], a = (c >> 24) > 0x80 ? 0x80 : (c >> 24);
        unsigned char rgb[3] = {(unsigned char)((c & 0xFF) * a / 0x80), (unsigned char)(((c >> 8) & 0xFF) * a / 0x80),
                                (unsigned char)(((c >> 16) & 0xFF) * a / 0x80)};
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

int main(int argc, char **argv) {
    FILE *f;
    char path[512];
    if (argc < 5) {
        fprintf(stderr, "buttons_art icons|hud PIXELS CLUT OUT_PREFIX\n");
        return 2;
    }
    f = fopen(argv[2], "rb");
    if (f == NULL || fread(px, 1, sizeof(px), f) != sizeof(px)) return 1;
    fclose(f);
    f = fopen(argv[3], "rb");
    if (f == NULL || fread(clut, 1, sizeof(clut), f) != sizeof(clut)) return 1;
    fclose(f);
    snprintf(path, sizeof(path), "%s_before.ppm", argv[4]);
    write_ppm(path);
    grx_buttons_make_xbox(strcmp(argv[1], "hud") == 0 ? GRX_BUTTONS_HUD : GRX_BUTTONS_ICONS, px, clut);
    snprintf(path, sizeof(path), "%s_after.ppm", argv[4]);
    write_ppm(path);
    return 0;
}
