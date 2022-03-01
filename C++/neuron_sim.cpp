#include "include/lib.h"
#include <numeric>
#include <execution>

#define V std::vector<int>

int N = 5;

std::vector<V> create_neurons_t1() {
    const std::vector<std::pair<int, int>> tuple_combinations = {{1, 2}, {1, 3}, {1, 4}, {2, 2}, {2, 3}};
    std::vector<V> c(N, V(1 << N, 0));
    for (size_t i = 0; i < N; ++i) {
        int mod = 1 << (N - i);
        for (size_t j = 0; j < (1 << N); ++j) {
            if (j % mod < mod / 2)
                c[i][j] = 1;
        }
    }

    std::vector<V> result;
    for (auto const &n_s : tuple_combinations) {
        for (auto const &combs : combinations(5, n_s)) {
            V layer(1 << N);
            for (size_t j = 0; j < (1 << N); ++j) {
                int ands = c[combs.first[0]][j];
                for (size_t i = 1; i < combs.first.size(); ++i)
                    ands &= c[combs.first[i]][j];
                int xors = c[combs.second[0]][j];
                for (size_t i = 1; i < combs.second.size(); ++i)
                    xors ^= c[combs.second[i]][j];
                layer[j] = (ands & xors) - (ands & (1 ^ xors));
            }
            result.push_back(layer);
        }
    }
    return result;
}

std::vector<V> create_neurons_t2() {
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

std::vector<V> create_neurons() {
    std::vector<V> v1 = create_neurons_t1();
    std::vector<V> v2 = create_neurons_t2();
    v1.insert(v1.end(), v2.begin(), v2.end());
    return v1;
}

std::vector<std::string> apply_things(std::vector<std::string> const &row, std::vector<V> const &neurons) {
    std::vector<std::string> result;
    for (auto const &neuron : neurons) {
        int weight = 0;
        for (int i = 2; i < row.size(); ++i) {
            int bit = row[i][0] - '0';
            weight += bit * neuron[i - 2];
        }
        result.push_back(std::to_string(weight));
    }
    return result;
}

int main() {
    int fd = open("result.csv", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    dup2(fd, 1);

    std::vector<V> neurons = create_neurons();
    std::cout << "Function,Class";
    for (int i = 0; i < neurons.size(); ++i) {
        std::cout << ","
                  << "n" << i;
    }
    std::cout << "\n";

    std::ifstream ifs;
    ifs.open("data/bitsshort.csv", std::ifstream::in);

    std::vector<std::string> row, result;
    std::string first_line;
    std::getline(ifs, first_line);
    while (ifs.good()) {
        row = get_row(ifs);
        if (row.size() < 2)
            continue;
        result = apply_things(row, neurons);
        std::cout << row[0] << "," << row[1] << ",";
        print_row(result);
    }
    ifs.close();
}