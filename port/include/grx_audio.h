#ifndef GRX_AUDIO_H
#define GRX_AUDIO_H

/*
 * Sound output: an SDL3 audio stream at 48 kHz stereo s16 on the default
 * playback device, pulling from the sound driver (grx_snd_render) on
 * SDL's audio thread. The stream's lock is the driver's lock.
 *
 * Call grx_audio_init once SDL is up (it initialises SDL's audio
 * subsystem itself) and before the game boots its IOP modules. Without a
 * device (or with GRX_AUDIO=0 in the environment) it returns -1 and the
 * driver keeps time on its own (grx_snd_poll), silently.
 */

int grx_audio_init(void);
void grx_audio_quit(void);

/* Stops or restarts the device (the window losing focus, say) */
void grx_audio_pause(int paused);

/* For a scene skip (grx_skip.h), when fields come faster than real time:
   with `on`, the device plays silence and the driver runs a field's worth
   of sound at each grx_audio_field (the frame loop's, once per field)
   instead, so the music and voices the scene waits on keep pace with the
   game. Without a device the driver keeps the wall clock's time. */
void grx_audio_by_fields(int on);
void grx_audio_field(void);

#endif
