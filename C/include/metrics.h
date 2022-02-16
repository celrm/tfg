#ifndef METRICS_H_
#define METRICS_H_
#define BitT char
#define LenT char
#define ExpT long long

int disorder_cube(const BitT * chain, LenT N);

int project(BitT ** p1, BitT ** p2, const BitT *chain, LenT N, LenT d);


#endif // METRICS_H_
