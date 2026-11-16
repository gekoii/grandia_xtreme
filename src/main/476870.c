/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void _motionVectors(s32 PMV[2][2][2], s32 *dmvector, s32 motion_vertical_field_select[2][2], s32 s, s32 motion_vector_count, s32 mv_format, s32 h_r_size, s32 v_r_size, s32 dmv, s32 mvscale) {
    if (motion_vector_count == 1) {
        if (mv_format == 0 && !dmv) {
            motion_vertical_field_select[1][s] = motion_vertical_field_select[0][s] = _getBits(1);
        }
        _motionVector(PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        PMV[1][s][0] = PMV[0][s][0];
        PMV[1][s][1] = PMV[0][s][1];
    } else {
        motion_vertical_field_select[0][s] = _getBits(1);
        _motionVector(PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        motion_vertical_field_select[1][s] = _getBits(1);
        _motionVector(PMV[1][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
    }
}
