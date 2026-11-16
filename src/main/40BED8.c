#include "common.h"

s32 UtilMatchKeyword(char **arg0, char *kw, s32 partial) {
    char *p;

    p = *arg0;
    while (*p == ' ' || *p == '\t') {
        p++;
    }
    while (*kw != 0) {
        if (*p != *kw) {
            return 0;
        }
        p++;
        kw++;
    }
    if (partial == 0) {
        if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') {
            return 0;
        }
    }
    *arg0 = p;
    return 1;
}
