/** 
 * name: strutils.h
 * author: danyou
 */

#pragma once

#include <stdlib.h>
#include <string.h>

#define UNIF_RAND() ((double)rand() / (RAND_MAX + 1.0))

/**
 * Discrete Random Sampling with Replacement
 * @param    size_t  n: Number of bins/length of p.
 * @param    double* p: Probability for each bin (must sum to 1).
 * @return   int     x: Index of the sampled bin.
 */
int DRSWR(size_t n, double *p)
{
    double *cdf = malloc(n*sizeof(double));
    if (cdf == NULL)
        return -1;
    memcpy(cdf, p, n*sizeof(double));
    for (size_t i = 1 ; i < n; i++)
        cdf[i] += cdf[i - 1];
    double rU = UNIF_RAND();
    int ans = n - 1;
    for (size_t j = 0; j < n; j++) {
        if (rU <= cdf[j]) {
            ans = j;
            break;
        }
    }
    free(cdf);
    return ans;
}
