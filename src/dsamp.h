/** 
 * name: dsamp.h
 * author: danyou
 * notes: 
 *   Insert `#define DSAMP_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef dsamp_h
#define dsamp_h

static inline size_t dsamp(size_t, const double*);

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
 * @return   size_t     x: Index of the sampled bin.
 */
static inline size_t dsamp(size_t n, const double *p)
{
    double *cdf = malloc(n*sizeof(double));
    memcpy(cdf, p, n*sizeof(double));
    for (size_t i = 1 ; i < n; i++)
        cdf[i] += cdf[i - 1];
    double uniformRand = ((double)rand() / (RAND_MAX + 1.0));
    size_t ans = n - 1;
    for (size_t j = 0; j < n; j++) {
        if (uniformRand <= cdf[j]) {
            ans = j;
            break;
        }
    }
    free(cdf);
    return ans;
}

#endif // DSAMP_IMPL
