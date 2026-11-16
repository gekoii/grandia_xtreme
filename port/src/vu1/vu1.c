/* The port's VU1: the game's one microprogram (vuMicroCode1), rewritten in
   C. grx_vu1_run() (grx_vu1.h) starts it at an entry point or continues it
   where its last phase ended; port/src/vif.c calls it for MSCAL and MSCNT
   at the point the VIF stream next waits for the VU (grx_vif.h).

   The program is a mesh renderer run in phases (port/docs/vu.md). Here
   each phase is a sequence of the stage functions in vu1_vertex.c and
   vu1_tri.c; a phase ends where the microcode has an E bit, and returns
   the address MSCNT continues from (the pair after the E bit's delay
   slot), so the flow follows the original's addresses:

     entry  phase 1                         then
     0x010  VU0 unlit: header, colours      0xB20 project listed, keys | 0xDF0 assemble
     0x030  VU0 lit: header, normals        0xC80 light | 0xCA0 project, keys | 0xDF0 assemble
     0x0D0+ VU1 unlit: header, positions,   0xBB0 project, keys | (0x2C30 sort) assemble
            colours (+0x10 x mode)
     0x110+ VU1 lit: header, positions      0xD48 normals | 0xD68 light | 0xD88 project,
                                            keys | (0x2C30 sort) assemble

   The entry picks the triangle stage (vi15): VU0 paths 0xD98 (no culling),
   VU1 mode 0 0x1048 (cull negative), mode 1 0x1200 (cull non-negative),
   modes 2 and 3 0x0ED8 (no culling).

   The VU0 paths' key pass ends with a branch whose delay slot holds the
   subroutine's jr (0x2AF8). Read as PCSX2 does (one pair at the branch
   target, then the jr), it returns to 0x0DE0 and the next MSCNT assembles
   unsorted; that is the default here. Build with
   -DGRX_VU1_VU0_PATH_SORTS=1 to take the other reading, where the program
   ends at 0x2C20 and the next MSCNT sorts first. */
#include <stdio.h>
#include "vu1_internal.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

#ifndef GRX_VU1_VU0_PATH_SORTS
#define GRX_VU1_VU0_PATH_SORTS 0
#endif

Vu1State grx_vu1;

/* Where MSCNT continues; NONE after a kick or before any program ran */
#define CONT_NONE 0xFFFFFFFFu
static uint32_t cont = CONT_NONE;

void vu1_warn(const char *what, uint32_t value) {
    static const char *seen[32];
    int i;
    for (i = 0; i < 32 && seen[i] != NULL; i++) {
        if (seen[i] == what) {
            return;
        }
    }
    if (i < 32) {
        seen[i] = what;
    }
    grx_log("vu1: %s (0x%X); further ones not reported", what, value);
}

/* The sort flag: bit 6 of the mode byte the CPU puts in the GIF tag's
   bits 32-39 (mtir vi13, vf19y; iand 0x40) */
static int sort_flag(void) {
    return (vu1_mtir(&grx_vu1.vf[VF_GIFTAG], Y) & 0x40) != 0;
}

/* Whether the key pass runs: the sort flag and more than one triangle
   (vi10 = triangle count - 1 is tested against zero) */
static int keys_run(void) {
    uint32_t ntri = vu1_ntri();
    if (ntri == 0) {
        vu1_warn("no triangles", 0); /* the key loop would run away */
        return 0;
    }
    return sort_flag() && ntri != 1;
}

/* The triangle stage set-up after the projection (jr vi15). Returns the
   continuation. */
static uint32_t triangle_stage(uint32_t pc) {
    switch (pc) {
    case 0x0D98: /* VU0 paths */
        if (keys_run()) {
            vu1_sort_keys_vu0();
            if (GRX_VU1_VU0_PATH_SORTS) {
                grx_vu1.sort_ret = 0x0DE0;
                return 0x2C30;
            }
        }
        return 0x0DF0;
    case 0x0ED8: /* VU1 modes 2, 3 */
    case 0x1048: /* VU1 mode 0 */
    case 0x1200: /* VU1 mode 1 */
        if (keys_run()) {
            vu1_sort_keys_vu1();
            grx_vu1.sort_ret = pc + 0x48; /* the stage's E pair: 0x0F20, 0x1090, 0x1248 */
            return 0x2C30;
        }
        return pc + 0x58; /* 0x0F30, 0x10A0, 0x1258 */
    default:
        vu1_warn("bad triangle stage", pc);
        return CONT_NONE;
    }
}

/* Runs from byte address pc to the next end; returns where MSCNT
   continues */
static uint32_t run(uint32_t pc) {
    uint32_t n;

    for (;;) {
        switch (pc) {
        /* init: vf31 = (0, 0, 0, 0), vf29 = (0, 0, 255, 0), from the two
           quadwords the block's own VIF stream unpacks to 0 and 1 */
        case 0x0000:
            grx_vu1.vf[VF_ZERO] = vu1_ld(0);
            grx_vu1.vf[VF_FOG] = vu1_ld(1);
            return 0x0168; /* a branch to itself */

        case 0x0010: /* VU0 unlit, phase 1 */
            grx_vu1.tri_stage = 0x0D98;
            vu1_load_header(HDR_VU0_UNLIT);
            vu1_pad_list();
            n = ((vu1_count() + 7) & 0xFC) - 4;
            if ((int32_t)n <= 0) {
                vu1_warn("colours: no vertices", vu1_count());
            } else {
                vu1_colours_to_float(VU1_COL, n);
            }
            return 0x0B20;

        case 0x0030: /* VU0 lit, phase 1 */
            grx_vu1.tri_stage = 0x0D98;
            vu1_load_header(HDR_VU0_LIT);
            vu1_pad_list();
            vu1_transform_normals_listed();
            return 0x0C80;

        case 0x00D0: /* VU1 unlit, phase 1 (mode 0..3) */
        case 0x00E0:
        case 0x00F0:
        case 0x0100:
            grx_vu1.tri_stage = pc == 0x00D0 ? 0x1048 : pc == 0x00E0 ? 0x1200 : 0x0ED8;
            vu1_load_header(HDR_VU1_UNLIT);
            n = (vu1_count() + 3) & 0xFC;
            if (n == 0) {
                vu1_warn("positions: no vertices", vu1_count());
            }
            vu1_transform_positions(n);
            vu1_colours_to_float(VU1_COL, n);
            return 0x0BB0;

        case 0x0110: /* VU1 lit, phase 1 (mode 0..3) */
        case 0x0120:
        case 0x0130:
        case 0x0140:
            grx_vu1.tri_stage = pc == 0x0110 ? 0x1048 : pc == 0x0120 ? 0x1200 : 0x0ED8;
            vu1_load_header(HDR_VU1_LIT);
            n = (vu1_count() + 3) & 0xFC;
            if (n == 0) {
                vu1_warn("positions: no vertices", vu1_count());
            }
            vu1_transform_positions(n);
            return 0x0D48;

        case 0x0C80: /* VU0 lit, phase 2 */
            vu1_light_listed();
            return 0x0CA0;

        case 0x0D48: /* VU1 lit, phase 2 */
            vu1_transform_normals_all();
            return 0x0D68;

        case 0x0D68: /* VU1 lit, phase 3 */
            vu1_light_all();
            return 0x0D88;

        case 0x0B20: /* VU0 paths: project (b L_0578), then the triangle stage */
        case 0x0CA0:
            vu1_project_listed();
            return triangle_stage(grx_vu1.tri_stage);

        case 0x0BB0: /* VU1 paths: project (b L_0810) */
        case 0x0D88:
            vu1_project_all();
            return triangle_stage(grx_vu1.tri_stage);

        case 0x2C30: /* the sort; it returns one pair past the stage's end */
            vu1_sort();
            pc = grx_vu1.sort_ret + 8;
            continue;

        case 0x0DE8: /* assemble, VU0 paths */
        case 0x0DF0:
            vu1_assemble(0, 0);
            return CONT_NONE;
        case 0x0F28: /* VU1 modes 2, 3 */
        case 0x0F30:
            vu1_assemble(1, 0);
            return CONT_NONE;
        case 0x1098: /* VU1 mode 0 */
        case 0x10A0:
            vu1_assemble(1, 1);
            return CONT_NONE;
        case 0x1250: /* VU1 mode 1 */
        case 0x1258:
            vu1_assemble(1, 2);
            return CONT_NONE;

        case 0x0020: /* dead entries, and init's end: branches to themselves */
        case 0x0040:
        case 0x0168:
            vu1_warn("program started in a self-loop (the VU would hang)", pc);
            return pc;

        default:
            vu1_warn("no program at this address", pc);
            return CONT_NONE;
        }
    }
}

void grx_vu1_run(uint32_t addr) {
    uint32_t pc;

    if (addr == GRX_VU1_CONTINUE) {
        if (cont == CONT_NONE) {
            vu1_warn("MSCNT with no program to continue", 0);
            return;
        }
        pc = cont;
    } else {
        pc = (addr * 8) & (GRX_VU1_MICRO_SIZE - 1); /* MSCAL counts pairs */
    }
    cont = run(pc);
}

/* Save states (grx_state_parts.h, through vif.c's part) */
void grx_vu1_state_io(GrxState *s) {
    GRX_STATE_VAR(s, grx_vu1);
    GRX_STATE_VAR(s, cont);
}
