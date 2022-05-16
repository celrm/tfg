#ifndef METRICS_H_
#define METRICS_H_

int disorder_cube(FunctionT const &chain);

int project(FunctionT &p1, FunctionT &p2, FunctionT const &chain, LenT d);

int disorder_CT(FunctionT const &chain);

int disorder_CT_equiv(FunctionT const &chain);

int chunks_k(int k, FunctionT const &chain);

int chunks_k_equiv(int k, FunctionT const &chain);

#endif // METRICS_H_
