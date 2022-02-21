#include <unordered_map>
#include "../include/utils.h"

BitT equal_string(const BitT *p1, const BitT *p2, LenT N) {
    for (ExpT i = 0; i < 1 << N; ++i)
        if (p1[i] != p2[i])
            return 1;
    return 0;
}

ExpT chain_to_num(const BitT *chain, LenT N) {
    ExpT result = 0;
    for (ExpT i = (1 << N) - 1; i >= 0; --i) {
        result *= 2;
        if(chain[i] != 0) result++;
    }
    return result;
}

int recursive_cube(std::unordered_map<ExpT,int> & used_beads, const BitT *chain, LenT N) {
    ExpT num = chain_to_num(chain,N);
    if(used_beads.find(num) != used_beads.end())
        return used_beads[num];
    if (N <= 0)
        return used_beads[num] = 1;

    BitT *p1, *p2;
    int m, metric = INT_MAX;

    for (LenT d = 0; d < N; ++d) {
        project(&p1, &p2, chain, N, d);
        if (equal_string(p1, p2, N - 1) == 0)
            m = recursive_cube(used_beads, p1, N - 1);
        else 
            m = recursive_cube(used_beads, p1, N - 1) + recursive_cube(used_beads, p2, N - 1);
        metric = m < metric ? m : metric;
        free(p1);
        free(p2);
    }

    used_beads[num] = metric;
    return metric;
}

int disorder_cube(const BitT *chain, LenT N) {
    std::unordered_map<ExpT,int> used_beads;
    return recursive_cube(used_beads,chain,N);
}

int project(BitT **p1, BitT **p2, const BitT *chain, LenT N, LenT d) {
    ExpT Dnm = 1 << (N - 1);
    BitT *pr1 = (BitT*) malloc(sizeof(BitT) * Dnm), *pr2 = (BitT*) malloc(sizeof(BitT) * Dnm);

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
    return 0;
}