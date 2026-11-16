/* Port-only C for SndEqFilterEnergy (0x00467BD4), which the decompilation still has in assembly. */
#include "common.h"

f32 SndEqFilterEnergy(s16 *samples, f32 *coef);

/* The energy of 2 x 512 samples (each the sum of samples[i] and
   samples[i + 0x200], the second block 0x800 bytes on) through six biquad
   sections in a row. Section k's coefficients are coef[5k..5k+4]: three
   feed-forward, two feedback, the feedback taps being the next section's
   inputs. The last section's feedback history (f21/f22) is only kept in
   the second block: in the first it stays 0, as in the asm. The asm saves
   $f20-$f23 in the scratchpad at 0x70002000-0x7000200F and restores them;
   that isn't carried over. */
f32 SndEqFilterEnergy(s16 *samples, f32 *coef) {
    /* h[k][0..1]: section k's last two inputs; h[6] the last outputs */
    f32 h[7][2];
    f32 in;
    f32 out;
    f32 acc;
    f32 energy = 0.0f;
    s32 pass;
    s32 i;
    s32 k;

    for (k = 0; k < 7; k++) {
        h[k][0] = 0.0f;
        h[k][1] = 0.0f;
    }
    for (pass = 0; pass < 2; pass++) {
        for (i = 0; i < 0x200; i++) {
            in = (f32)(samples[0x200] + samples[0]);
            samples++;
            for (k = 0; k < 6; k++) {
                const f32 *c = &coef[k * 5];

                acc = in * c[0];
                acc = acc + h[k][0] * c[1];
                acc = acc + h[k][1] * c[2];
                h[k][1] = h[k][0];
                h[k][0] = in;
                acc = acc + h[k + 1][0] * c[3];
                out = acc + h[k + 1][1] * c[4];
                in = out;
            }
            if (pass != 0) {
                h[6][1] = h[6][0];
                h[6][0] = out;
            }
            energy = energy + out * out;
        }
        samples += 0x200;
    }
    return energy;
}
