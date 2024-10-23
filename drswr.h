/** 
 * name: drswr.h
 * author: danyou
 * notes: 
 *   Insert `#define DRSWR_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef drswr_h
#define drswr_h

static inline size_t DRSWR(size_t, const double*);

#endif // drswr_h

// Implementation:

#ifdef DRSWR_IMPL
#undef DRSWR_IMPL

#include <stdlib.h>
#include <string.h>
/**
 * Discrete Random Sampling with Replacement
 * @param    size_t     n: Number of bins/length of p (min. 1).
 * @param    double*    p: Probability for each bin (must sum to 1).
 * @return   size_t     x: Index of the sampled bin.
 */
static inline size_t drswr(size_t n, const double *p)
{
    double *cdf = malloc(n*sizeof(double));
    if (cdf == NULL)
        exit(EXIT_FAILURE);
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

#endif // DSAMPLR_IMPL
