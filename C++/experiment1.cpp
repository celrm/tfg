#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "include/lib.h"

int main() {
    srand(time(NULL));

    int fd = open("result.csv", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    dup2(fd, 1);

    LenT N = 5; // 5 in 1 bit functions
    ExpT circuits_per_size = 10000;
    LenT max_size = 7;

    printf("size,function,metric\n");
    for (LenT size = 3; size <= max_size; ++size) {
        CircuitT circuit(size, std::vector<std::vector<LenT>>(size, std::vector<LenT>(2, 0)));
        std::vector<FunctionT> functions(size, FunctionT(1 << N, 0));
        for (ExpT i = 0; i < circuits_per_size; ++i) {
            random_circuit(circuit, N);
            // print_circuit(circuit, N, size, size);
            get_functions(functions, circuit);

            for (LenT j = 0; j < size; ++j) {
                printf("%d,", size);
                for (ExpT input = 0; input < (1 << N); ++input) {
                    printf("%d", functions[j][input]);
                }
                printf(",%d", disorder_cube(functions[j]));
                printf("\n");
            }
        }
    }
}