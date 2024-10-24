#include <stdio.h>
#include <stdlib.h>

#define DSAMP_IMPL
#include "dsamp.h"

int main(void)
{
    static const size_t n = 3;
    double p[n] = {0.2, 0.7, 0.1};

    size_t nans = 100;
    size_t counts[n] = {0};

    for (size_t i = 0; i < nans; i++) {
        size_t sample;
        dsamp(n, p, &sample);
        counts[sample] += 1;
    }

    printf("Stats:\n");
    printf("No. Zeroes: %f\n", (float)counts[0]/nans);
    printf("No. Ones: %f\n", (float)counts[1]/nans);
    printf("No. Twos: %f\n", (float)counts[2]/nans);
    return 0;
}