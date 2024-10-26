/** 
 * name: dsamp.h
 * author: danyou
 * notes: 
 *   Insert `#define DSAMP_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef dsamp_h
#define dsamp_h

int dsamp(const size_t, const double*, size_t* const);

#endif // dsamp_h

// Implementation:

#ifdef DSAMP_IMPL
#undef DSAMP_IMPL

#include <stdlib.h>
#include <string.h>
/**
 * Discrete Random Sampling with Replacement
 * @param    size_t     n: Number of bins/length of p (min. 1).
 * @param    double*    p: Probability for each bin (must sum to 1).
 * @param    size_t*    x: Pointer for index of the sampled bin.
 * @return   int        _: 1 upon success and 0 upon error.
 */
int dsamp(size_t n, const double *p, size_t* const x)
{
    double *cdf = malloc(n*sizeof(double));
    if (cdf == NULL)
        return 0;
    memcpy(cdf, p, n*sizeof(double));
    for (size_t i = 1; i < n; i++)
        cdf[i] += cdf[i - 1];
    double uniformRand = ((double)rand() / (RAND_MAX + 1.0));
    *x = n - 1;
    for (size_t j = 0; j < n; j++) {
        if (uniformRand <= cdf[j]) {
            *x = j;
            break;
        }
    }
    free(cdf);
    return 1;
}

#endif // DSAMP_IMPL
