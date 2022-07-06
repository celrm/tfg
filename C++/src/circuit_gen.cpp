#include "../include/lib.h"
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <stdexcept>

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

FunctionT basic_function(LenT w, int N) {
    FunctionT f(1 << N);
    for (ExpT input = 0; input < (1 << N); ++input) {
        f[input] = (input >> w) & 1;
    }
    return f;
}

int mutual_endogamy(CircuitT const &c1, CircuitT const &c2, std::vector<FunctionT> &f1, std::vector<FunctionT> &f2, int N) {
    LenT Dp = c1.size();
    LenT Wd = c1[0].size();
    CircuitT values1(Dp, std::vector<std::vector<LenT>>(Wd, std::vector<LenT>(1 << N, 0)));
    CircuitT values2(Dp, std::vector<std::vector<LenT>>(Wd, std::vector<LenT>(1 << N, 0)));
    MatrixT values(Dp, std::vector<LenT>(Wd, 0));
    
    // fill values1 and values2 with all gate evaluations for each input
    for (ExpT input = 0; input < (1 << N); ++input) { // all possible inputs 000-111
        evaluate_circuit(input, c1, values);
        for (LenT d = 0; d < Dp; ++d) {
            for (LenT w = 0; w < Wd; ++w) {
                values1[d][w][input] = values[d][w];
            }
        }
        for (LenT w = 0; w < Wd; ++w) {
            f1[w][input] = values[Dp - 1][w];
        }
        evaluate_circuit(input, c2, values);
        for (LenT d = 0; d < Dp; ++d) {
            for (LenT w = 0; w < Wd; ++w) {
                values2[d][w][input] = values[d][w];
            }
        }
        for (LenT w = 0; w < Wd; ++w) {
            f2[w][input] = values[Dp - 1][w];
        }
    }
    // get gates used in the circuits
    std::set<std::vector<LenT>> funcs1, funcs2;
    std::unordered_set<LenT> current_inputs1, current_inputs2;
    current_inputs1.insert(0);
    current_inputs2.insert(0);

    for (LenT d = Dp-1; d >=0; --d) {
        std::unordered_set<LenT> used_inputs1, used_inputs2;
        for (auto w : current_inputs1) {
            funcs1.insert(values1[d][w]);
            used_inputs1.insert(c1[d][w][0]);
            used_inputs1.insert(c1[d][w][1]);
        }
        for (auto w : current_inputs2) {
            funcs2.insert(values2[d][w]);
            used_inputs2.insert(c1[d][w][0]);
            used_inputs2.insert(c1[d][w][1]);
        }
        current_inputs1 = used_inputs1;
        current_inputs2 = used_inputs2;
    }
    // insert inputs as also functions
    for (auto w : current_inputs1) {
        funcs1.insert(basic_function(w,N));
    }
    for (auto w : current_inputs2) {
        funcs2.insert(basic_function(w,N));
    }
    
    // get size of intersection
    LenT size = 0;
    for (auto it = funcs1.begin(); it != funcs1.end(); ++it) {
        if (funcs2.find(*it) != funcs2.end()) {
            ++size;
        }
    }

    return size;
}

// class set
template <typename T>
class myset {
public:
    myset() {}
    void insert(T i) {
        if (_set.find(i) == _set.end()) {
            _set.insert({i,1});
        }
        else {
            ++_set[i];
        }
    }
    void insert(T i, int n) {
        if (_set.find(i) == _set.end()) {
            _set.insert({i,n});
        }
        else {
            _set[i] += n;
        }
    }
    void erase(T i) {
        if (_set.find(i) != _set.end()) {
            --_set[i];
            if (_set[i] == 0) {
                _set.erase(i);
            }
        }
    }
    bool contains(T i) const {
        return _set.find(i) != _set.end();
    }
    int operator[](T i) const {
        return _set[i];
    }
    int& operator[](T i) {
        return _set[i];
    }
    typename std::map<T,int>::iterator begin() {
        return _set.begin();
    }
    typename std::map<T,int>::iterator end() {
        return _set.end();
    }
    typename std::map<T,int>::iterator find(T i) {
        return _set.find(i);
    }
private:
    std::map<T,int> _set;
};

double divide_double(double a, LenT b) {
    if (b < 0) {
        throw std::invalid_argument( "received negative value" );
    }
    if (b == 0) {
        return a;
    }
    if (a < 0) {
        return 0;
    }
    return divide_double(a / 2.0q , b-1);
}

double mutual_endogamy2(CircuitT &c1, CircuitT &c2, std::vector<FunctionT> &f1, std::vector<FunctionT> &f2, int N) {
    LenT Dp = c1.size();
    LenT Wd = c1[0].size();
    
    // simplification
    std::vector<std::vector<LenT>> ig1(Dp, std::vector<LenT>(Wd, -1));
    std::vector<std::vector<LenT>> ig2(Dp, std::vector<LenT>(Wd, -1));
    for (LenT wa = 0; wa < Wd; ++wa) {
        if (ig1[0][wa] == -1) {
            ig1[0][wa] = wa;
            for (LenT wb = wa+1; wb < Wd; ++wb) {
                if ((c1[0][wa][0] == c1[0][wb][0] && c1[0][wa][1] == c1[0][wb][1])
                || (c1[0][wa][0] == c1[0][wb][1] && c1[0][wa][1] == c1[0][wb][0])) {
                    ig1[0][wb] = wa;
                }
            }
        }
        if (ig2[0][wa] == -1) {
            ig2[0][wa] = wa;
            for (LenT wb = wa+1; wb < Wd; ++wb) {
                if ((c1[0][wa][0] == c1[0][wb][0] && c1[0][wa][1] == c1[0][wb][1])
                || (c1[0][wa][0] == c1[0][wb][1] && c1[0][wa][1] == c1[0][wb][0])) {
                    ig1[0][wb] = wa;
                }
                if ((c2[0][wa][0] == c2[0][wb][0] && c2[0][wa][1] == c2[0][wb][1])
                || (c2[0][wa][0] == c2[0][wb][1] && c2[0][wa][1] == c2[0][wb][0])) {
                    ig2[0][wb] = wa;
                }
            }
        }
    }

    for (LenT d = 1; d < Dp; ++d) {
        for (LenT w = 0; w < Wd; ++w) {
            c1[d][w][0] = ig1[d-1][c1[d][w][0]];
            c1[d][w][1] = ig1[d-1][c1[d][w][1]];
            c2[d][w][0] = ig2[d-1][c2[d][w][0]];
            c2[d][w][1] = ig2[d-1][c2[d][w][1]];
        }
        for (LenT w1 = 0; w1 < Wd; ++w1) {
            if (ig1[d][w1] == -1) {
                ig1[d][w1] = w1;
                for (LenT w2 = w1+1; w2 < Wd; ++w2) {
                    if ((c1[d][w1][0] == c1[d][w2][0] && c1[d][w1][1] == c1[d][w2][1])
                    || (c1[d][w1][0] == c1[d][w2][1] && c1[d][w1][1] == c1[d][w2][0])) {
                        ig1[d][w2] = w1;
                    }
                }
            }
            if (ig2[d][w1] == -1) {
                ig2[d][w1] = w1;
                for (LenT w2 = w1+1; w2 < Wd; ++w2) {
                    if ((c2[d][w1][0] == c2[d][w2][0] && c2[d][w1][1] == c2[d][w2][1])
                    || (c2[d][w1][0] == c2[d][w2][1] && c2[d][w1][1] == c2[d][w2][0])) {
                        ig2[d][w2] = w1;
                    }
                }
            }
        }
    }

    // syntactic equivalence
    std::vector<std::vector<std::vector<bool>>> iguales(Dp, std::vector<std::vector<bool>>(Wd, std::vector<bool>(Wd, false)));
    for (LenT w1 = 0; w1 < Wd; ++w1) {
        LenT h11 = c1[0][w1][0];
        LenT h12 = c1[0][w1][1];
        for (LenT w2 = 0; w2 < Wd; ++w2) {
            LenT h21 = c2[0][w2][0];
            LenT h22 = c2[0][w2][1];
            if ((h11 == h21 && h12 == h22) || 
                (h11 == h22 && h12 == h21)) {
                iguales[0][w1][w2] = true;
            }
        }
    }
    for (LenT d = 1; d < Dp; ++d) {
        for (LenT w1 = 0; w1 < Wd; ++w1) {
            LenT h11 = c1[d][w1][0];
            LenT h12 = c1[d][w1][1];
            for (LenT w2 = 0; w2 < Wd; ++w2) {
                LenT h21 = c2[d][w2][0];
                LenT h22 = c2[d][w2][1];
                if ((iguales[d-1][h11][h21] && iguales[d-1][h12][h22]) ||
                    (iguales[d-1][h11][h22] && iguales[d-1][h12][h21])) {
                    iguales[d][w1][w2] = true;
                }
            }
        }
    }
    double res = 0;
    // get gates used in the circuits
    myset<LenT> in1, in2;
    in1.insert(0);
    in2.insert(0);

    for (LenT d = Dp-1; d >=0; --d) {
        myset<LenT> used_inputs1, used_inputs2;
        std::vector<LenT> b1(Wd,0), b2(Wd,0);
        for (auto w1p : in1) {
            LenT w1 = w1p.first;
            for (auto w2p : in2) {
                LenT w2 = w2p.first;
                if (iguales[d][w1][w2]) {
                    double add = divide_double(std::min(w1p.second,w2p.second), Dp - 1 - d);
                    res += add;
                    b1[w1]++;
                    b2[w2]++;
                }
            }
            if (b1[w1] == 0) {
                used_inputs1.insert(c1[d][w1][0], w1p.second);
                used_inputs1.insert(c1[d][w1][1], w1p.second);
            }
        }
        for (auto w2p : in2) {
            LenT w2 = w2p.first;
            if (b2[w2] == 0) {
                used_inputs2.insert(c2[d][w2][0], w2p.second);
                used_inputs2.insert(c2[d][w2][1], w2p.second);
            }
        }
        in1 = used_inputs1;
        in2 = used_inputs2;
    }

    // insert inputs as also functions
    for (auto w1p : in1) {
        LenT w1 = w1p.first;
        if (in2.find(w1) != in2.end()) {
            res += divide_double(std::min(w1p.second,in2[w1]), Dp);
        }
    }

    return res;
}


int mean_question_height(std::vector<std::vector<int>> & q1, std::vector<std::vector<int>> & q2) {
    LenT N = q1.size();
    int result = 0;
    for (LenT i = 0; i < N; ++i) {
        int mean1 = 0;
        for (LenT j = 0; j < q1[i].size(); ++j) {
            mean1 += q1[i][j];
        }
        mean1 = q1[i].size() == 0 ? 0 : mean1 / q1[i].size();

        int mean2 = 0;
        for (LenT j = 0; j < q2[i].size(); ++j) {
            mean2 += q2[i][j];
        }
        mean2 = q2[i].size() == 0 ? 0 : mean2 / q2[i].size();

        result += abs(mean1 - mean2);
    }
    return result;
}