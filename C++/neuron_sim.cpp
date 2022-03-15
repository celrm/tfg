#include "include/lib.h"
#include <numeric>
#include <execution>
#include <unordered_map>

#define V std::vector<int>

int N = 5;
std::unordered_map<int, V> c;
static int init() {
    for (size_t i = 0; i < N; ++i) {
        c[i] = V(1 << N, 0);
        c[i + N] = V(1 << N, 0);
        int mod = 1 << (N - i);
        for (size_t j = 0; j < (1 << N); ++j) {
            if (j % mod < mod / 2)
                c[i][j] = 1;
            else
                c[i + N][j] = 1;
        }
    }
    return 0;
}

std::vector<V> create_neurons_xors() {
    const std::vector<std::pair<int, int>> tuple_combinations = {{1, 2}, {1, 3}, {1, 4}, {2, 2}, {2, 3}};
    std::vector<V> result;
    for (auto const &n_s : tuple_combinations) {
        for (auto const &combs : combinations(5, n_s)) {
            V layer(1 << N);
            for (size_t j = 0; j < (1 << N); ++j) {
                int ands = 1;
                for (size_t i = 0; i < combs.first.size(); ++i)
                    ands &= c[combs.first[i]][j];

                int xors = c[combs.second[0]][j]; // xors != 0
                for (size_t i = 1; i < combs.second.size(); ++i)
                    xors ^= c[combs.second[i]][j];

                layer[j] = (ands & xors) - (ands & (1 ^ xors));
            }
            result.push_back(layer);
        }
    }
    return result;
}

std::vector<V> negative_combinations(V const &s) {
    std::vector<V> result(1 << s.size(), V(s.size(), 0));
    for (int i = 0; i < (1 << s.size()); ++i) {
        for (int j = 0; j < s.size(); ++j) {
            if (i & (1 << j)) // if bit j is 1 in i
                result[i][j] = s[j];
            else
                result[i][j] = s[j] + N; // negative
        }
    }
    return result;
}

std::vector<V> create_neurons_projections() {
    std::vector<V> result;
    for (size_t i = 0; i < N; ++i) {
        for (auto const &pos_combs : combinations(5, {i, 0})) {
            for (auto const &combs : negative_combinations(pos_combs.first)) {
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

std::vector<V> create_neurons_radial() {
    std::vector<V> result;
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

std::vector<std::string> apply_things(std::vector<std::string> const &row, std::vector<V> const &neurons, int (*func)(int, int)) {
    std::vector<std::string> result;
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

int main() {
    std::vector<std::string> row, result_xors, result_radial, result_projections;
    int fd = open("result.csv", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    dup2(fd, 1);
    init();

    std::ifstream ifs;
    ifs.open("./bitsDataset.csv", std::ifstream::in); // be careful with size

    row = get_row(ifs); // header
    print_row(row, ",", "");

    std::vector<V> neurons_xors = create_neurons_xors();
    for (int i = 0; i < neurons_xors.size(); ++i)
        std::cout << ",x" << i;
    std::vector<V> neurons_radial = create_neurons_radial();
    for (int i = 0; i < neurons_radial.size(); ++i)
        std::cout << ",r" << i;
    std::vector<V> neurons_projections = create_neurons_projections();
    for (int i = 0; i < neurons_projections.size(); ++i)
        std::cout << ",p" << i;
    std::cout << "\n";
    
    while (ifs.good()) {
        row = get_row(ifs);
        if (row.size() < 2) continue;
        print_row(row, ",", ","); // bits

        result_xors = apply_things(row, neurons_xors, &apply_product);
        print_row(result_xors, ",", ",");
        result_radial = apply_things(row, neurons_radial, &apply_product);
        print_row(result_radial, ",", ",");
        result_projections = apply_things(row, neurons_projections, &apply_countbits);
        print_row(result_projections);
    }
    ifs.close();
}