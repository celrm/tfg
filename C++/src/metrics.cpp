#include <unordered_map>
#include <set>
#include "../include/lib.h"

template <typename T>
int bit_width(T x) {
    unsigned int exp = x;
    int log = 0;
    while (exp >>= 1)
        ++log;
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

//////////////// Cube ////////////////

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
    d = N - d - 1;
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

///////////////// CT ////////////////

int disorder_CT(FunctionT const &chain) {
    if (chain.size() <= 1)
        return 1;

    FunctionT p1(chain.size() / 2, 0), p2(chain.size() / 2, 0);
    project(p1, p2, chain, 0);

    if (p1 == p2)
        return disorder_CT(p1);
    else
        return disorder_CT(p1) + disorder_CT(p2);
}

int apply_permutation(std::vector<int> const &perm, int i) {
    int result = 0;
    for (LenT j = 0; j < perm.size(); ++j) {
        j *= 2;
        j += (1 << j) & i;
    }
    return result;
}

int disorder_CT_equiv(FunctionT const &chain) {
    LenT N = bit_width(chain.size());
    int metric = INT_MAX;

    std::vector<int> perm(N);
    for (LenT i = 0; i < N; ++i)
        perm[i] = i;

    do { // N! permutations
        FunctionT sigma_chain(chain.size(), 0);
        for (ExpT i = 0; i < chain.size(); ++i) {
            int j = apply_permutation(perm, i);
            sigma_chain[i] = chain[j];
        }
        int m = disorder_CT(sigma_chain);
        metric = m < metric ? m : metric;
    } while (std::next_permutation(perm.begin(), perm.end()));
    return metric;
}

///////////////// Chunks ////////////////
int chunks_k(int k, FunctionT const &chain) {
    LenT N = bit_width(chain.size());

    std::set<FunctionT> chunks;
    for (LenT i = 0; i < (1 << (N - k)); ++i) {
        FunctionT chunk((1 << k), 0);
        chunk.assign(chain.begin() + i * (1 << k), chain.end() + (i + 1) * (1 << k));
        chunks.insert(chunk);
    }
    return chunks.size();
}

int chunks_k_equiv(int k, FunctionT const &chain) {
    LenT N = bit_width(chain.size());
    int metric = INT_MAX;

    std::vector<int> perm(N);
    for (LenT i = 0; i < N; ++i)
        perm[i] = i;

    do { // N! permutations
        FunctionT sigma_chain(chain.size(), 0);
        for (ExpT i = 0; i < chain.size(); ++i) {
            int j = apply_permutation(perm, i);
            sigma_chain[i] = chain[j];
        }
        int m = chunks_k(k, sigma_chain);
        metric = m < metric ? m : metric;
    } while (std::next_permutation(perm.begin(), perm.end()));
    return metric;
}