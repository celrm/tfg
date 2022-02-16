#include <stdlib.h> // \n( )*\{
#include <stdio.h>
#include <limits.h>
#define BitT char
#define LenT char
#define ExpT long long

#include "../include/metrics.h"

BitT equal_string(const BitT *p1, const BitT *p2, LenT N) {
    for (ExpT i = 0; i < 1 << N; ++i)
        if (p1[i] != p2[i])
            return 1;
    return 0;
}

int disorder_cube(const BitT *chain, LenT N) {
    if (N <= 0)
        return 1;
    BitT *p1, *p2;
    int p, metric = INT_MAX;

    for (LenT d = 0; d < N; ++d) {
        project(&p1, &p2, chain, N, d);

        if (equal_string(p1, p2, N - 1) == 0)
            p = disorder_cube(p1, N - 1);
        else
            p = disorder_cube(p1, N - 1) + disorder_cube(p2, N - 1);
        metric = p < metric ? p : metric;

        free(p1);
        free(p2);
    }
    return metric;
}

int project(BitT **p1, BitT **p2, const BitT *chain, LenT N, LenT d) {
    ExpT Dnm = 1 << (N - 1);
    BitT *pr1 = malloc(sizeof(BitT) * Dnm), *pr2 = malloc(sizeof(BitT) * Dnm);

    for (ExpT i = 0; i < 2 * Dnm; ++i) {
        // From i, takes bit in position d to the most significant (N-1)
        ExpT ni = i % (1 << d) + ((i >> d + 1) << d) + ((i >> d) % 2 << N - 1);
        if (ni < Dnm)
            pr1[ni] = chain[i];
        else
            pr2[ni % Dnm] = chain[i];
    }
    *p1 = pr1;
    *p2 = pr2;
}