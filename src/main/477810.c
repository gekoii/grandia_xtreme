/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"


void _updateTemporalReferenceTackingData(void) {
    s32 t;

    if ((picture_coding_type != 3) && (temporal_reference != temporal_reference_old)) {
        if (temporal_reference_wrap != 0) {
            temporal_reference_wrap = 0;
            Temporal_Reference_Base += 0x400;
        }
        if (temporal_reference < temporal_reference_old) {
            if (Temporal_Reference_GOP_Reset == 0) {
                temporal_reference_wrap = 1;
            }
        }
        Temporal_Reference_GOP_Reset = 0;
        temporal_reference_old = temporal_reference;
    }
    True_Framenum = Temporal_Reference_Base + temporal_reference;
    if (temporal_reference_wrap != 0 && temporal_reference_old >= temporal_reference) {
        True_Framenum += 0x400;
    }
    True_Framenum_max = (True_Framenum_max < True_Framenum) ? True_Framenum : True_Framenum_max;
}
