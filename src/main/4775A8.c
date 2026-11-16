/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

#define IPU_CTRL (*(volatile u32 *)0x10002010)



void _pictureCodingExtension(void) {
    SceMpegSys *p;

    p = _sceMpegCurMp->sys;
    f_code[0][0] = _getBits(4);
    f_code[0][1] = _getBits(4);
    f_code[1][0] = _getBits(4);
    f_code[1][1] = _getBits(4);
    intra_dc_precision = _getBits(2);
    IPU_CTRL = (IPU_CTRL & 0xFFFCFFFF) | (intra_dc_precision << 16);
    picture_structure = _getBits(2);
    if (p->firstFieldStructure == 0) {
        p->firstFieldStructure = picture_structure;
    }
    top_field_first = _getBits(1);
    frame_pred_frame_dct = _getBits(1);
    concealment_motion_vectors = _getBits(1);
    q_scale_type = _getBits(1);
    IPU_CTRL = (IPU_CTRL & 0xFFBFFFFF) | (q_scale_type << 22);
    intra_vlc_format = _getBits(1);
    IPU_CTRL = (IPU_CTRL & 0xFFDFFFFF) | (intra_vlc_format << 21);
    alternate_scan = _getBits(1);
    IPU_CTRL = (IPU_CTRL & 0xFFEFFFFF) | (alternate_scan << 20);
    repeat_first_field = _getBits(1);
    chroma_420_type = _getBits(1);
    progressive_frame = _getBits(1);
    composite_display_flag = _getBits(1);
    if (composite_display_flag != 0) {
        v_axis = _getBits(1);
        field_sequence = _getBits(3);
        sub_carrier = _getBits(1);
        burst_amplitude = _getBits(7);
        sub_carrier_phase = _getBits(8);
    }
}
