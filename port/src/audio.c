/*
 * Sound output through SDL3 (port/include/grx_audio.h): an SDL_AudioStream
 * at 48 kHz stereo s16, the SPU2's own rate and format, so SDL only
 * converts if the device needs it. The stream's callback runs the sound
 * driver on SDL's audio thread with the stream locked; the driver's
 * game-thread entry points take the same lock (grx_snd_set_lock).
 */

#include <stdint.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

#include "grx_audio.h"
#include "grx_snd.h"
#include "grx_platform.h"

static SDL_AudioStream *stream;
static SDL_AtomicInt by_fields; /* grx_audio_by_fields */

static void lock(void *ctx) {
    SDL_LockAudioStream((SDL_AudioStream *)ctx);
}

static void unlock(void *ctx) {
    SDL_UnlockAudioStream((SDL_AudioStream *)ctx);
}

/* SDL wants `additional` more bytes; the stream is locked */
static void SDLCALL feed(void *userdata, SDL_AudioStream *s, int additional, int total) {
    int16_t buf[2 * 512];

    (void)userdata;
    (void)total;
    if (SDL_GetAtomicInt(&by_fields)) {
        /* The driver runs from grx_audio_field meanwhile: silence */
        SDL_memset(buf, 0, sizeof(buf));
        while (additional > 0) {
            int n = additional < (int)sizeof(buf) ? additional : (int)sizeof(buf);
            if (!SDL_PutAudioStreamData(s, buf, n)) {
                break;
            }
            additional -= n;
        }
        return;
    }
    while (additional > 0) {
        int frames = (additional + 3) / 4;
        if (frames > 512) {
            frames = 512;
        }
        grx_snd_render(buf, frames);
        if (!SDL_PutAudioStreamData(s, buf, frames * 4)) {
            break;
        }
        additional -= frames * 4;
    }
}

int grx_audio_init(void) {
    const SDL_AudioSpec spec = { .format = SDL_AUDIO_S16, .channels = 2, .freq = 48000 };
    const char *env = getenv("GRX_AUDIO");

    if (grx_snd_init() != 0) {
        grx_log("audio: out of memory for the SPU2");
        return -1;
    }
    if (env != NULL && env[0] == '0') {
        grx_log("audio: off (GRX_AUDIO=0)");
        return -1;
    }
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        grx_log("audio: no audio (%s)", SDL_GetError());
        return -1;
    }
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, feed, NULL);
    if (stream == NULL) {
        grx_log("audio: no playback device (%s)", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return -1;
    }
    grx_snd_set_lock(lock, unlock, stream);
    grx_snd_set_realtime(1);
    if (!SDL_ResumeAudioStreamDevice(stream)) {
        grx_log("audio: device won't start (%s)", SDL_GetError());
    }
    grx_log("audio: 48000 Hz stereo on %s", SDL_GetCurrentAudioDriver());
    return 0;
}

void grx_audio_quit(void) {
    if (stream == NULL) {
        return;
    }
    SDL_PauseAudioStreamDevice(stream);
    grx_snd_set_lock(NULL, NULL, NULL);
    grx_snd_set_realtime(0);
    SDL_DestroyAudioStream(stream);
    stream = NULL;
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void grx_audio_pause(int paused) {
    if (stream == NULL) {
        return;
    }
    if (paused) {
        SDL_PauseAudioStreamDevice(stream);
    } else {
        SDL_ResumeAudioStreamDevice(stream);
    }
}

void grx_audio_by_fields(int on) {
    SDL_SetAtomicInt(&by_fields, on);
}

void grx_audio_field(void) {
    static unsigned rem;
    int16_t buf[2 * 512];
    int n;

    if (stream == NULL || !SDL_GetAtomicInt(&by_fields)) {
        return;
    }
    /* 48000 frames a second at 60000/1001 fields a second */
    rem += 48000u * 1001u;
    n = (int)(rem / 60000u);
    rem %= 60000u;
    SDL_LockAudioStream(stream);
    while (n > 0) {
        int k = n > 512 ? 512 : n;
        grx_snd_render(buf, k);
        n -= k;
    }
    SDL_UnlockAudioStream(stream);
}
