/* Matches newlib's _setlocale_r (locale.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

struct _reent {
    u8 pad[0x30];
    s32 _current_category;
    const char *_current_locale;
};

extern char __localeCStr[];
extern char __localeEmptyStr[];
extern s32 strcmp(const char *, const char *);

char *_setlocale_r(struct _reent *p, s32 category, const char *locale) {
    if (locale) {
        if (strcmp(locale, __localeCStr) && strcmp(locale, __localeEmptyStr))
            return 0;
        p->_current_category = category;
        p->_current_locale = locale;
    }
    return __localeCStr;
}
