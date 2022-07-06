#include <unordered_map>
#include <set>
#include "../include/lib.h"
#define V std::vector<int>
#define M std::vector<V>

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

int recursive_cube(FunctionT const &chain) {
    ExpT num = chain_to_num(chain);
    if (chain.size() <= 1)
        return 1;

    FunctionT p1(chain.size() / 2, 0), p2(chain.size() / 2, 0);
    int m, metric = INT_MAX;

    LenT N = bit_width(chain.size());
    for (LenT d = 0; d < N; ++d) {
        project(p1, p2, chain, d);
        if (p1 == p2)
            m = recursive_cube(p1);
        else
            m = recursive_cube(p1) + recursive_cube(p2);
        metric = m < metric ? m : metric;
    }
    return metric;
}

int disorder_cube(FunctionT const &chain) {
    return recursive_cube(chain);
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


int recursive_cube2(FunctionT const &chain, M & questions, std::set<LenT> & answers, int N, int len) {
    ExpT num = chain_to_num(chain);
    if (chain.size() <= 1)
        return 1;

    FunctionT p1(chain.size() / 2, 0), p2(chain.size() / 2, 0);
    int m, metric = INT_MAX;

    M this_layer_questions;
    LenT final_d = -1;
    for (LenT d = 0; d < N; ++d) {
        if (answers.find(d) != answers.end())
            continue;
        auto it = answers.insert(d);
        LenT pos = std::distance(answers.begin(), it.first);
        project(p1, p2, chain, d-pos);
        M new_questions = questions;
        bool equal = p1 == p2;
        if (equal)
            m = recursive_cube2(p1, new_questions, answers, N,len);
        else
            m = recursive_cube2(p1, new_questions, answers, N,len/2) + 
                recursive_cube2(p2, new_questions, answers, N,len/2);
        answers.erase(d);
        if (m < metric) {
            metric = m;
            this_layer_questions = new_questions;
            if (!equal) {
                final_d = d;
            }
            else {
                final_d = -1;
            }
        }
    }
    if(final_d == -1) {
        questions = this_layer_questions;
    }
    else {
        questions = this_layer_questions;
        questions[final_d].push_back(len);
    }
    return metric;
}

int disorder_cube2(FunctionT const &chain, M & questions) {
    std::set<LenT> answers;

    LenT N = bit_width(chain.size());
    int metric = recursive_cube2(chain, questions, answers, N,1<<N);
    return metric;
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

///////////////// Chunks ////////////////

int disorder_chunks_k(int k, FunctionT const &chain) {
    LenT N = bit_width(chain.size());

    std::unordered_map<ExpT,int> chunks;
    for (LenT i = 0; i < (1 << (N - k)); ++i) {
        FunctionT chunk((1 << k), 0);
        chunk.assign(chain.begin() + i * (1 << k), chain.begin() + (i + 1) * (1 << k));
        chunks[chain_to_num(chunk)] = 1;
    }
    return chunks.size();
}

///////////////// All equivalent chains ////////////////

int apply_permutation(std::vector<int> const &perm, LenT i) {
    LenT result = 0;
    for (LenT j = perm.size() - 1; j >= 0; --j) {
        result *= 2;
        result += ((1 << perm[j]) & i) > 0 ? 1 : 0;
    }
    return result;
}

std::vector<FunctionT> all_sigma_chains(FunctionT const &chain) {
    std::vector<FunctionT> result;
    LenT N = bit_width(chain.size());
    
    std::vector<int> perm(N);
    for (LenT i = 0; i < N; ++i) {
        perm[i] = i;
    }
    do { // N! permutations
        FunctionT sigma_chain(chain.size(), 0);
        for (ExpT i = 0; i < chain.size(); ++i) {
            int j = apply_permutation(perm, i);
            sigma_chain[i] = chain[j];
        }
        result.push_back(sigma_chain);
    } while (std::next_permutation(perm.begin(), perm.end()));
    return result;
}