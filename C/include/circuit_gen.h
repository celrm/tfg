#ifndef CIRCUIT_GEN_H_
#define CIRCUIT_GEN_H_
#define BitT char
#define LenT char
#define ExpT long long

int print_circuit(const LenT *circuit, LenT N, LenT Depth, LenT Width);

int random_circuit(LenT ** circuit, LenT N, LenT Depth, LenT Width);

int evaluate_circuit(ExpT input, const LenT *circuit, LenT Depth, LenT Width, BitT *values);

int get_function(BitT **function, const LenT *circuit, LenT N, LenT Depth, LenT Width);

#endif // CIRCUIT_GEN_H_
