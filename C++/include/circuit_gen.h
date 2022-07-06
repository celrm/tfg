#ifndef CIRCUIT_GEN_H_
#define CIRCUIT_GEN_H_

int print_circuit(CircuitT const &circuit);

int random_circuit(CircuitT &circuit, LenT N);

int evaluate_circuit(ExpT input, CircuitT const &circuit, MatrixT &values);

int get_functions(std::vector<FunctionT> &functions, CircuitT const &circuit);

int mutual_endogamy(CircuitT const &c1, CircuitT const &c2, std::vector<FunctionT> &f1, std::vector<FunctionT> &f2, int N);
double mutual_endogamy2(CircuitT &c1, CircuitT &c2, std::vector<FunctionT> &f1, std::vector<FunctionT> &f2, int N);

int mean_question_height(std::vector<std::vector<int>> & q1, std::vector<std::vector<int>> & q2);

#endif // CIRCUIT_GEN_H_
