#ifndef NOISE_H
#define NOISE_H

#include <math.h>
#include <stdlib.h>

/** improved perlin noise
 * straight forward from the reference implementation:
 *    http://mrl.nyu.edu/~perlin/noise/
 */
double noise3d(double x, double y, double z);

#endif
