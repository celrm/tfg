#include "../include/lib.h"

int print_row(std::vector<std::string> const &row, std::string const &delim, std::string const &final) {
    for (int i = 0; i < row.size(); ++i) {
        std::cout << row[i];
        if (i != row.size() - 1)
            std::cout << delim;
    }
    std::cout << final;
    return 0;
}

std::vector<std::string> get_row(std::ifstream &ifs) {
    std::vector<std::string> result;
    std::string line, cell;
    std::getline(ifs, line);
    std::stringstream lineStream(line);

    while (std::getline(lineStream, cell, ','))
        result.push_back(cell);
    if (!lineStream && cell.empty())
        result.push_back("");
    return result;
}

std::vector<std::vector<std::vector<int>>> combinations(int n, std::vector<int> const &n_s) {
    std::vector<int> v(n);
    int count = 0;
    for (size_t i = 0; i < n_s.size(); i++) {
        std::fill(v.end() - n_s[i] - count, v.end() - count, n_s.size()-i);
        count += n_s[i];
    }

    std::vector<std::vector<std::vector<int>>> result;
    do {
        std::vector<std::vector<int>> combs(n_s.size());
        for (int i = 0; i < n; ++i) {
            if (v[i] != 0)
                combs[n_s.size()-v[i]].push_back(i);
        }
        result.push_back(combs);
    } while (std::next_permutation(v.begin(), v.end()));
    return result;
}

std::vector<std::vector<int>> negative_combinations(int (*neg)(int), std::vector<int> const &s) {
    std::vector<std::vector<int>> result(1 << s.size(), std::vector<int>(s.size(), 0));
    for (int i = 0; i < (1 << s.size()); ++i) {
        for (int j = 0; j < s.size(); ++j) {
            if (i & (1 << j)) // if bit j is 1 in i
                result[i][j] = s[j];
            else
                result[i][j] = neg(s[j]); // negative
        }
    }
    return result;
}
