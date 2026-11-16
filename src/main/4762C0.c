/* rodata: 0x004A7600-0x004A7620 */
/* Follows the MSSG mpeg2decode reference decoder (motion vector / macroblock
 * decoding), adapted for the PS2's IPU. Third-party code: it falls under that
 * decoder's license; see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0x13C - 8];
    s32 unk13C;
} E;

typedef struct {
    E e[2];
    s32 idx;
} G;

extern s32 Fault_Flag;
extern s32 picture_coding_type;
extern s32 full_pel_forward_vector;
extern s32 forward_f_code;
extern s32 full_pel_backward_vector;
extern s32 backward_f_code;
extern s32 f_code[2][2];
extern s32 picture_structure;
extern s32 frame_pred_frame_dct;
extern s32 concealment_motion_vectors;
extern s32 _sceMpegDcReset;
extern s32 quantizer_scale_code;
extern G _sceMpegMbSlots;
extern s32 MPEG2_Flag;

void _motionVectors(s32 PMV[2][2][2], s32 *dmvector, s32 motion_vertical_field_select[2][2], s32 s, s32 motion_vector_count, s32 mv_format, s32 h_r_size, s32 v_r_size, s32 dmv, s32 mvscale);

s32 _decodeMacroblock(s32 *macroblock_type, s32 *motion_type, s32 *dct_type, s32 PMV[2][2][2], s32 mvfs[2][2], s32 *dmvector) {
    s32 motion_vector_count, mv_format, dmv, mvscale;
    E *e;
    u32 dcr;
    u32 c;

    *(volatile u32 *)0x10002010 = (*(volatile u32 *)0x10002010 & 0xF8FFFFFF) | (picture_coding_type << 24);
    *macroblock_type = _sceMpegIpuVdec(1);
    if (*macroblock_type == 0) {
        _sceMpegError("Invalid macroblock_type code: 0");
        Fault_Flag = 1;
        return 0;
    }

    if (*macroblock_type & 0xC) {
        if (picture_structure == 3 && frame_pred_frame_dct) {
            *motion_type = 2;
        } else {
            *motion_type = _getBits(2);
        }
    } else if ((*macroblock_type & 1) && concealment_motion_vectors) {
        *motion_type = (picture_structure == 3) ? 2 : 1;
    }

    if (picture_structure == 3) {
        motion_vector_count = (*motion_type == 1) ? 2 : 1;
        mv_format = (*motion_type == 2);
    } else {
        motion_vector_count = (*motion_type == 2) ? 2 : 1;
        mv_format = 0;
    }
    dmv = (*motion_type == 3);
    mvscale = (mv_format == 0 && picture_structure == 3);
    *dct_type = (picture_structure == 3 && !frame_pred_frame_dct && (*macroblock_type & 3)) ? _getBits(1) : 0;

    if (*macroblock_type & 0x10) {
        quantizer_scale_code = _getBits(5);
    }
    if ((*macroblock_type & 8) || ((*macroblock_type & 1) && concealment_motion_vectors)) {
        if (MPEG2_Flag) {
            _motionVectors(PMV, dmvector, mvfs, 0, motion_vector_count, mv_format, f_code[0][0] - 1, f_code[0][1] - 1, dmv, mvscale);
        } else {
            _motionVector(PMV[0][0], dmvector, forward_f_code - 1, forward_f_code - 1, 0, 0, full_pel_forward_vector);
        }
    }
    if (Fault_Flag) {
        return 0;
    }
    if (*macroblock_type & 4) {
        if (MPEG2_Flag) {
            _motionVectors(PMV, dmvector, mvfs, 1, motion_vector_count, mv_format, f_code[1][0] - 1, f_code[1][1] - 1, 0, mvscale);
        } else {
            _motionVector(PMV[0][1], dmvector, backward_f_code - 1, backward_f_code - 1, 0, 0, full_pel_backward_vector);
        }
    }
    if (Fault_Flag) {
        return 0;
    }
    if ((*macroblock_type & 1) && concealment_motion_vectors) {
        _flushBuffer(1);
    }
    if (*macroblock_type & 3) {
        e = &_sceMpegMbSlots.e[_sceMpegMbSlots.idx];
        *(volatile u32 *)0x1000B010 = (e->unk4 & 0x0FFFFFFF) | 0x80000000;
        *(volatile u32 *)0x1000B020 = 0x30;
        *(volatile u32 *)0x1000B000 = 0x100;
        _sceMpegIpuWait();
        c = ((*macroblock_type & 1) << 27) | (quantizer_scale_code << 16);
        dcr = (_sceMpegDcReset << 26) | 0x20000000;
        _sceMpegIpuCmd(c | dcr | (*dct_type << 25));
    } else {
        _sceMpegMbSlots.e[_sceMpegMbSlots.idx].unk13C = 1;
    }
    _sceMpegDcReset = 0;
    if (Fault_Flag) {
        return 0;
    }
    if (!(*macroblock_type & 1)) {
        _sceMpegDcReset = 1;
    }
    if ((*macroblock_type & 1) && !concealment_motion_vectors) {
        PMV[0][0][0] = PMV[0][0][1] = PMV[1][0][0] = PMV[1][0][1] = 0;
        PMV[0][1][0] = PMV[0][1][1] = PMV[1][1][0] = PMV[1][1][1] = 0;
    }
    if (picture_coding_type == 2 && !(*macroblock_type & 9)) {
        PMV[0][0][0] = PMV[0][0][1] = PMV[1][0][0] = PMV[1][0][1] = 0;
        if (picture_structure == 3) {
            *motion_type = 2;
        } else {
            *motion_type = 1;
            mvfs[0][0] = (picture_structure == 2);
        }
    }
    return 1;
}
