/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n14.h"

void MapSplineSolve(s32 h, f32 *y, s32 n, f32 *m) {
    s32 i;
    f32 r;

    n--;
    for (i = 1; i < n; i++) {
        m[i] = ((y[i + 1] - y[i]) / (f32)(h * h) - (y[i] - y[i - 1]) / (f32)(h * h)) * 6.0f;
        mapSplineTriUpper[i] = 1.0f;
        mapSplineTriDiag[i] = 4.0f;
    }
    mapSplineTriDiag[n] = 1.0f;
    mapSplineTriUpper[n] = 1.0f;
    m[n] = 0.0f;
    mapSplineTriDiag[n - 1] += 1.0f;
    mapSplineTriUpper[1] /= mapSplineTriDiag[1];
    m[1] /= mapSplineTriDiag[1];
    for (i = 2; i <= n; i++) {
        r = 1.0f / (mapSplineTriDiag[i] - mapSplineTriUpper[i - 1]);
        mapSplineTriUpper[i] *= r;
        m[i] = (m[i] - m[i - 1]) * r;
    }
    for (i = n - 1; i > 0; i--) {
        m[i] -= m[i + 1] * mapSplineTriUpper[i];
    }
    m[0] = 0.0f;
    m[n] = 0.0f;
}
