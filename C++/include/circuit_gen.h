#ifndef CIRCUIT_GEN_H_
#define CIRCUIT_GEN_H_

int print_circuit(CircuitT const &circuit);

int random_circuit(CircuitT &circuit, LenT N);

int evaluate_circuit(ExpT input, CircuitT const &circuit, MatrixT &values);

int get_functions(std::vector<FunctionT> &functions, CircuitT const &circuit);

#endif // CIRCUIT_GEN_H_
