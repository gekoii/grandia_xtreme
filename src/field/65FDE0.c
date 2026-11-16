#include "common.h"
#include "field/n26.h"

void strcpy(char *, char *);

void McLoadSetErrorMsg(char *s) {
    strcpy(mcLoadErrorMsg, s);
}
