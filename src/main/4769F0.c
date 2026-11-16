/* Follows the MSSG mpeg2decode reference decoder (motion vector / macroblock
 * decoding), adapted for the PS2's IPU. Third-party code: it falls under that
 * decoder's license; see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void _motionVector(s32 *PMV, s32 *dmvector, s32 h_r_size, s32 v_r_size, s32 dmv, s32 mvscale, s32 full_pel_vector) {
    s32 motion_code, motion_residual;

    motion_code = _sceMpegIpuVdec(2);
    motion_residual = (h_r_size != 0 && motion_code != 0) ? _getBits(h_r_size) : 0;
    _decodeMotionVector(&PMV[0], h_r_size, motion_code, motion_residual, full_pel_vector);
    if (dmv)
        dmvector[0] = _getDmvector();

    motion_code = _sceMpegIpuVdec(2);
    motion_residual = (v_r_size != 0 && motion_code != 0) ? _getBits(v_r_size) : 0;
    if (mvscale)
        PMV[1] >>= 1;
    _decodeMotionVector(&PMV[1], v_r_size, motion_code, motion_residual, full_pel_vector);
    if (mvscale)
        PMV[1] <<= 1;
    if (dmv)
        dmvector[1] = _getDmvector();
}
