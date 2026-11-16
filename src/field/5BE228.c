#include "common.h"
#include "sys.h"
#include "field/event.h"
#include "field/n13.h"

void OccMovieForward(u8 *arg0) {
    u8 v = OCC_PARAMS(arg0, OccMovie)->movie;

    sysState.movieNo = v;
    sysState.progChangeReq = 1;
}
