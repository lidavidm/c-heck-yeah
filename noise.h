#ifndef NOISE
#define NOISE

#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#define NUM_FREQUENCIES 32
double* noise(int seed, size_t size);

#endif
