/* Third-party library code (newlib): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's mprec.c: pow5mult's p05
 * (48A240.c) and the powers of ten used by _strtod_r (483350.c) and
 * _dtoa_r. Data-only C (plan step 6b, docs/shifting.md). The tables are
 * newlib's tens/bigtens/tinytens (#defined to the __mprec_ names). Split
 * from 4A8BF8.c (locale.c) at the original file boundary. Don't include the
 * project headers here.
 */
/* rodata: 0x004A8C40-0x004A8D68 */

/* pow5mult's local static */
const int p05[3] = { 5, 25, 125 };

const double __mprec_tens[] = {
    1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
    1e20, 1e21, 1e22, 1e23, 1e24,
};

/* fields: D_004A8D38 [4] */
const double __mprec_bigtens[] = { 1e16, 1e32, 1e64, 1e128, 1e256 };

/* the plain 1e-256 (not newlib's IEEE_Arith 9007199254740992.e-256) */
const double __mprec_tinytens[] = { 1e-16, 1e-32, 1e-64, 1e-128, 1e-256 };
