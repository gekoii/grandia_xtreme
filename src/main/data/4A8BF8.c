/* Third-party library code (newlib): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's locale.c: the C locale's lconv
 * (returned by _localeconv_r, 488DA0.c), its strings, and the "C" that
 * _setlocale_r (488D18.c) accepts and returns. Data-only C (plan step 6b,
 * docs/shifting.md). gcc 2.9 emits the struct, then its new literals last
 * first ("" then "."); their labels (__localeEmptyStr, __localeDecimalPoint) resolve
 * against the file's __rodata_ marker. _setlocale_r also compares with "",
 * which it reuses. The .data half of locale.c (__mb_cur_max and the
 * LC_CTYPE names) is in 4A0498.c. Don't include the project headers here.
 */
/* rodata: 0x004A8BF8-0x004A8C40 */

#define CHAR_MAX 127

struct lconv {
    char *decimal_point;
    char *thousands_sep;
    char *grouping;
    char *int_curr_symbol;
    char *currency_symbol;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
};

/* newlib's static lconv */
const struct lconv _lconv = {
    ".", "", "", "", "", "", "", "", "", "",
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
};

/* _setlocale_r's "C" literal */
const char __localeCStr[] = "C";
