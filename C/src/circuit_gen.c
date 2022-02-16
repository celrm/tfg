#include <stdlib.h> // \n( )*\{
#include <stdio.h>
#define BitT char
#define LenT char
#define ExpT long long

#include "../include/circuit_gen.h"

int print_circuit(const LenT *circuit, LenT N, LenT Depth, LenT Width) {
    for (LenT d = 0; d < Depth; ++d) {
        for (LenT w = 0; w < Width; ++w) {
            printf("%d %d | ", circuit[d * Width + w * 2], circuit[d * Width + w * 2 + 1]);
        }
        printf("\n");
    }
}

int random_circuit(LenT **circuit, LenT N, LenT Depth, LenT Width) {
    LenT *c = malloc(sizeof(LenT) * (Depth * Width * 2));
    for (LenT d = 0; d < Depth; ++d) {
        for (LenT w = 0; w < Width; ++w) {
            int index = d * Width + w * 2;
            c[index] = rand() % (d == 0 ? N : Width);     // parent 0 index, from input or above layer
            c[index + 1] = rand() % (d == 0 ? N : Width); // parent 1 index, from input or above layer
        }
    }
    *circuit = c;
}

int evaluate_circuit(ExpT input, const LenT *circuit, LenT Depth, LenT Width, BitT *values) {
    LenT p0_i, p1_i;
    BitT p0, p1;
    for (LenT d = 0; d < Depth; ++d) {
        for (LenT w = 0; w < Width; ++w) {
            p0_i = circuit[d * Width + w * 2];     // circuit[d][w][0]
            p1_i = circuit[d * Width + w * 2 + 1]; // circuit[d][w][1]
            if (d == 0) {
                p0 = (input >> p0_i) & 1; // seek parent 0 from input: input[p0_i]
                p1 = (input >> p1_i) & 1; // seek parent 1 from input: input[p1_i]
            }
            else {
                p0 = values[(d - 1) * Width + p0_i]; // from above layer: values[d-1][p0_i]
                p1 = values[(d - 1) * Width + p1_i]; // from above layer: values[d-1][p1_i]
            }
            values[d * Width + w] = 1 ^ (p0 & p1);
        }
    }
}

int get_function(BitT **function, const LenT *circuit, LenT N, LenT Depth, LenT Width) {
    BitT *f = malloc(sizeof(BitT) * (1 << N) * Width);
    BitT *values = malloc(sizeof(BitT) * (Depth * Width));

    for (ExpT input = 0; input < (1 << N); ++input) { // all possible inputs 000-111
        evaluate_circuit(input, circuit, Depth, Width, values);
        for (LenT w = 0; w < Width; ++w) {
            f[w * (1 << N) + input] = values[(Depth - 1) * Width + w]; // f[w][input] = values[D-1][w]
        }
    }
    free(values);
    *function = f;
}