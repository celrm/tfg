#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "include/utils.h"

int endogamy(const LenT *circuit, LenT N, LenT Depth, LenT Width, int j) {
    return 0;
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