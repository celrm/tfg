#include "../include/utils.h"

int print_circuit(CircuitT const &circuit) {
    for (LenT d = 0; d < circuit.size(); ++d) {
        for (LenT w = 0; w < circuit[0].size(); ++w) {
            printf("%d %d | ", circuit[d][w][0], circuit[d][w][1]);
        }
        printf("\n");
    }
    return 0;
}

int random_circuit(CircuitT &circuit, LenT N) {
    for (LenT d = 0; d < circuit.size(); ++d) {
        for (LenT w = 0; w < circuit[0].size(); ++w) {
            circuit[d][w][0] = rand() % (d == 0 ? N : circuit[0].size()); // parent 0 index, from input or above layer
            circuit[d][w][1] = rand() % (d == 0 ? N : circuit[0].size()); // parent 1 index, from input or above layer
        }
    }
    return 0;
}

int evaluate_circuit(ExpT input, CircuitT const &circuit, MatrixT &values) {
    LenT p0_i, p1_i;
    BitT p0, p1;
    for (LenT d = 0; d < circuit.size(); ++d) {
        for (LenT w = 0; w < circuit[0].size(); ++w) {
            p0_i = circuit[d][w][0];
            p1_i = circuit[d][w][1];
            if (d == 0) {
                p0 = (input >> p0_i) & 1; // seek parent 0 from input: input[p0_i]
                p1 = (input >> p1_i) & 1; // seek parent 1 from input: input[p1_i]
            }
            else {
                p0 = values[d - 1][p0_i]; // from above layer: values[d-1][p0_i]
                p1 = values[d - 1][p1_i]; // from above layer: values[d-1][p1_i]
            }
            values[d][w] = 1 ^ (p0 & p1);
        }
    }
    return 0;
}

int get_functions(std::vector<FunctionT> &functions, CircuitT const &circuit) {
    MatrixT values(circuit.size(), std::vector<LenT>(circuit[0].size(), 0));

    for (ExpT input = 0; input < functions[0].size(); ++input) { // all possible inputs 000-111
        evaluate_circuit(input, circuit, values);
        for (LenT w = 0; w < functions.size(); ++w) {
            functions[w][input] = values[circuit.size() - 1][w];
        }
    }
    return 0;
}