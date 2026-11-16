/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


double atof(const char *arg0) {
    return ((double (*)(const char *, char **))strtod)(arg0, 0);
}
