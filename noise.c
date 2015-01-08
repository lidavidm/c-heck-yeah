#include "noise.h"

// Pink noise implementation
// http://www.redblobgames.com/articles/noise/introduction.html
double* noise(int seed, size_t size) {
    srand(seed);

    double *result = malloc(sizeof(double) * size);
    for (size_t x = 0; x < size; x++) { result[x] = 0.0; }

    // In referenced link: weighted_sum
    // (amplitude and noise functions folded inline)
    for (size_t k = 1; k <= NUM_FREQUENCIES; k++) {
        double phase = ((double) rand()) / ((RAND_MAX) / (2 * M_PI));

        for (size_t x = 0; x < size; x++) {
            double amplitude = 1.0 / k;

            // make sure noise is always positive
            double noise = 1.0 + sin(2*M_PI * k*x/size + phase);
            result[x] += amplitude * noise;
        }
    }

    return result;
}
