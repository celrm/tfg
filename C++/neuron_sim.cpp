#include "include/lib.h"
#include <numeric>
#include <execution>
#include <unordered_map>

#define V std::vector<int>
#define M std::vector<V>
#define R std::vector<std::string>

static int N = 5;
std::unordered_map<int, V> c;

static int neg(int i) {
    return i + N;
}

static int init() {
    for (size_t i = 0; i < N; ++i) {
        c[i] = V(1 << N, 0);
        c[neg(i)] = V(1 << N, 0);
        int mod = 1 << (N - i);
        for (size_t j = 0; j < (1 << N); ++j) {
            if (j % mod < mod / 2)
                c[i][j] = 1;
            else
                c[neg(i)][j] = 1;
        }
    }
    return 0;
}

M create_neurons_xors() {
    const M tuple_combinations = {{1, 2}, {1, 3}, {1, 4}, {2, 2}, {2, 3}};
    M result;
    for (auto const &n_s : tuple_combinations) {
        for (auto const &pos_combs : combinations(5, n_s)) {
            auto const xor_combs = pos_combs[1];
            for (auto const &and_combs : negative_combinations(&neg, pos_combs[0])) {
                V layer(1 << N);
                V neg_layer(1 << N); // single bifurcation of xor_combs here
                for (size_t j = 0; j < (1 << N); ++j) {
                    int ands = 1; // neutral element
                    for (size_t i = 0; i < and_combs.size(); ++i)
                        ands &= c[and_combs[i]][j];

                    int xors = 0; // neutral element
                    for (size_t i = 0; i < xor_combs.size(); ++i)
                        xors ^= c[xor_combs[i]][j];

                    layer[j] = (ands & xors) - (ands & (1 ^ xors));
                    neg_layer[j] = (ands & (1 ^ xors)) - (ands & xors);
                }
                result.push_back(layer);
                result.push_back(neg_layer);
            }
        }
    }
    return result;
}

M create_neurons_projections() {
    M result;
    for (int i = 0; i < N; ++i) {
        for (auto const &pos_combs : combinations(5, {i})) {
            for (auto const &combs : negative_combinations(&neg, pos_combs[0])) {
                V layer(1 << N);
                for (size_t j = 0; j < (1 << N); ++j) {
                    int ands = 1;
                    for (size_t i = 0; i < combs.size(); ++i)
                        ands &= c[combs[i]][j];
                    layer[j] = ands;
                }
                result.push_back(layer);
            }
        }
    }
    return result;
}

M create_neurons_radial() {
    M result;
    for (size_t i = 0; i < (1 << N); ++i) {
        V layer(1 << N, 0);

        layer[i] = 4;

        // distance 1
        for (int d1 = 0; d1 < N; ++d1) {
            int j = i ^ (1 << d1);
            layer[j] = -2;
        }

        // distance 2
        for (int d2 = 0; d2 < N; ++d2) {
            int j = i ^ (1 << d2);
            layer[j] = 1;
        }

        result.push_back(layer);
    }
    return result;
}

static int apply_product(int b, int mask) {
    return b * mask;
}

static int apply_countbits(int b, int mask) {
    return (b - (1 ^ b)) * mask;
}

R apply_things(R const &row, M const &neurons, int (*func)(int, int)) {
    R result;
    for (auto const &neuron : neurons) {
        int weight = 0;
        for (int i = 2; i < row.size(); ++i) {
            int bit = row[i][0] - '0';
            weight += func(bit, neuron[i - 2]);
        }
        result.push_back(std::to_string(weight));
    }
    return result;
}

int main(int argc,  char** argv) {
    if(argc != 4 || strlen(argv[3]) != 4) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file> <neuron_type(nxrp)>" << std::endl;
        std::cout << "Example: " << argv[0] << " input.txt output.txt n-r-" << std::endl;
        exit(EXIT_FAILURE);
    }
    bool is_n = argv[3][0] == 'n';
    bool is_x = argv[3][1] == 'x';
    bool is_r = argv[3][2] == 'r';
    bool is_p = argv[3][3] == 'p';

    R row;
    M neurons_xors, neurons_radial, neurons_projections;
    int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644); // "result.csv"
    dup2(fd, 1);
    init();

    std::ifstream ifs;
    ifs.open(argv[1], std::ifstream::in); // be careful with size // "./bitsDataset.csv"

    row = get_row(ifs); // header
    if(is_n) print_row(row);
    else print_row({row[0],row[1]});

    if(is_x) {
        neurons_xors = create_neurons_xors();
        for (int i = 0; i < neurons_xors.size(); ++i)
            std::cout << ",x" << i;
    }
    if(is_r) {
        neurons_radial = create_neurons_radial();
        for (int i = 0; i < neurons_radial.size(); ++i)
            std::cout << ",r" << i;
    }
    if(is_p) {
        neurons_projections = create_neurons_projections();
        for (int i = 0; i < neurons_projections.size(); ++i)
            std::cout << ",p" << i;
    }
    std::cout << std::endl;

    while (ifs.good()) {
        row = get_row(ifs);
        if (row.size() < 2) continue;
        if (row[1] == "Small" || row[1] == "0") row[1] = "0";
        else if (row[1] == "Big" || row[1] == "1") row[1] = "1";

        if(is_n) print_row(row);
        else print_row({row[0],row[1]});

        if(is_x) {
            std::cout << ",";
            print_row(apply_things(row, neurons_xors, apply_product));
        }
        if(is_r) {
            std::cout << ",";
            print_row(apply_things(row, neurons_radial, apply_countbits));
        }
        if(is_p) {
            std::cout << ",";
            print_row(apply_things(row, neurons_projections, apply_countbits));
        }
        std::cout << std::endl;
    }
    ifs.close();
}