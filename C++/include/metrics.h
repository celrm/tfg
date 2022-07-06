#ifndef METRICS_H_
#define METRICS_H_

int disorder_cube(FunctionT const &chain);

int disorder_cube2(FunctionT const &chain, std::vector<std::vector<int>> & questions);

int project(FunctionT &p1, FunctionT &p2, FunctionT const &chain, LenT d);

int disorder_CT(FunctionT const &chain);

int disorder_chunks_k(int k, FunctionT const &chain);

std::vector<FunctionT> all_sigma_chains(FunctionT const &chain);

#endif // METRICS_H_
