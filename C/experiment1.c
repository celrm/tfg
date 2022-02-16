#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "include/circuit_gen.h"

int main() {
    srand(time(NULL));

    int fd = open("result.csv", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    dup2(fd, 1);

    LenT N = 5; // 5 in 1 bit functions
    ExpT circuits_per_size = 1000;
    LenT max_size = 7;
    LenT *circuit;
    BitT *functions;

    printf("size,function\n");
    for (LenT size = 3; size <= max_size; ++size) {
        for (ExpT i = 0; i < circuits_per_size; ++i) {
            random_circuit(&circuit, N, size, size);
            // print_circuit(circuit, N, size, size);
            get_function(&functions, circuit, N, size, size);

            for(LenT j=0; j<size;++j) {
                printf("%d,", size);
                for (ExpT input = 0; input < (1 << N); ++input) {
                    printf("%d", functions[j*(1 << N) + input]); // f[j][input]
                }
                printf("\n");
            }
            
            free(circuit);
            free(functions);
        }
    }
}