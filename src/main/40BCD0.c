#include "common.h"

void UtilReadWord(char **pp, char *out, s32 skipline) {
    while (**pp == ' ' || **pp == '\t') {
        (*pp)++;
    }
    while (**pp != ' ' && **pp != '\t' && **pp != '\n' && **pp != '\r') {
        *out++ = **pp;
        (*pp)++;
    }
    *out = 0;
    if (skipline) {
        while (**pp != '\n') {
            (*pp)++;
        }
        (*pp)++;
    }
}
