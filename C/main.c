#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#define BitT char
#define LenT char
#define ExpT long long

#include "include/metrics.h"
#include "include/circuit_gen.h"

int endogamy(const LenT *circuit, LenT N, LenT Depth, LenT Width, int j) {
    if(Depth == 1) return 1;

    LenT *values = malloc(sizeof(LenT) * ((Depth-1) * Width + N));
    memset(values, 0, sizeof(LenT) * ((Depth-1) * Width + N));
    
    LenT p0_i = circuit[(Depth-1) * Width + j * 2];     // circuit[Depth-1][w][0]
    LenT p1_i = circuit[(Depth-1) * Width + j * 2 + 1]; // circuit[Depth-1][w][1]
    values[N + (Depth-2) * Width + p0_i] += 1;
    values[N + (Depth-2) * Width + p1_i] += 1;

    for (LenT d = Depth-2; d >=0; --d) {
        for (LenT w = 0; w < Width; ++w) {
            LenT v = values[N + d * Width + w];
            p0_i = circuit[d * Width + w * 2];     // circuit[d][w][0]
            p1_i = circuit[d * Width + w * 2 + 1]; // circuit[d][w][1]
            if (d == 0) {
                values[p0_i] += v;
                values[p1_i] += v;
            }
            else {
                values[N + (d-1) * Width + p0_i] += v;
                values[N + (d-1) * Width + p1_i] += v; // (p0_i==p1_i) ? 0 : 
            }
        }
    }
    int ret = 0;
    for (LenT w = 0; w < N; ++w) {
        ret = ret > values[w] ? ret : values[w];
    }
    free(values);
    return ret;
}

int main() {
    srand(time(NULL));

    LenT *circuit;
    BitT *functions, *f;

    LenT N = 5; // 5 in 1 bit functions
    LenT size = 3;

    random_circuit(&circuit, N, size, size);
    print_circuit(circuit, N, size, size);
    get_function(&functions, circuit, N, size, size);

    for(LenT j=0; j<size;++j) {
        f = functions + j*(1<<N);
        for (ExpT input = 0; input < (1 << N); ++input) {
            printf("%d", f[input]); // f[j][input]
        }
        printf("\t%d",disorder_cube(f,N));
        printf("\t%d",endogamy(circuit,N,size,size,j));
        printf("\n");
    }
    free(circuit);
    free(functions);
}