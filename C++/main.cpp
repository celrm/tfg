#include "include/utils.h"

int main() {
    srand(time(NULL));

    LenT N = 5; // 5 in 1 bit functions
    LenT size = 3;

    CircuitT circuit(size,std::vector<std::vector<LenT> >(size,std::vector<LenT>(2,0)));
    std::vector<FunctionT> functions(N,FunctionT(1<<N,0));

    random_circuit(circuit,N);
    print_circuit(circuit);
    get_function(functions, circuit);

    for(LenT j=0; j<size;++j) {
        for (ExpT input = 0; input < (1 << N); ++input) {
            printf("%d", functions[j][input]);
        }
        // printf("\t%d",disorder_cube(functions[j]));
        printf("\n");
    }
}