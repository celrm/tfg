#include <unordered_map>
#include "../include/lib.h"

template <typename T>
int bit_width(T x) {
    unsigned int exp = x;
    int log = 0;
    while (exp >>= 1) ++log;
    return log;
}

ExpT chain_to_num(FunctionT const &chain) {
    ExpT result = 0;
    for (ExpT i = 0; i < chain.size(); ++i) {
        result *= 2;
        if (chain[i] != 0)
            result++;
    }
    return result;
}

int recursive_cube(std::unordered_map<ExpT, int> &used_beads, FunctionT const &chain) {
    ExpT num = chain_to_num(chain);
    if (used_beads.find(num) != used_beads.end())
        return used_beads[num];
    if (chain.size() <= 1)
        return used_beads[num] = 1;

    FunctionT p1(chain.size() / 2, 0), p2(chain.size() / 2, 0);
    int m, metric = INT_MAX;

    LenT N = bit_width(chain.size());
    for (LenT d = 0; d < N; ++d) {
        project(p1, p2, chain, d);
        if (p1 == p2)
            m = recursive_cube(used_beads, p1);
        else
            m = recursive_cube(used_beads, p1) + recursive_cube(used_beads, p2);
        metric = m < metric ? m : metric;
    }
    return used_beads[num] = metric;
}

int disorder_cube(FunctionT const &chain) {
    std::unordered_map<ExpT, int> used_beads;
    return recursive_cube(used_beads, chain);
}

int project(FunctionT &p1, FunctionT &p2, FunctionT const &chain, LenT d) {
    LenT N = bit_width(chain.size());
    for (ExpT i = 0; i < chain.size(); ++i) {
        // From i, takes bit in position d to the most significant (N-1)
        ExpT ni = i % (1 << d) + ((i >> d + 1) << d) + ((i >> d) % 2 << N - 1);
        if (ni < p1.size())
            p1[ni] = chain[i];
        else
            p2[ni % p1.size()] = chain[i];
    }
    return 0;
}