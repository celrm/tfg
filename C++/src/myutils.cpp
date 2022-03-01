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

std::vector<std::pair<std::vector<int>, std::vector<int>>> combinations(int n, std::pair<int, int> const &n_s) {
    std::vector<int> v(n);
    std::fill(v.end() - n_s.first, v.end(), 2);
    std::fill(v.end() - n_s.first - n_s.second, v.end() - n_s.first, 1);

    std::vector<std::pair<std::vector<int>, std::vector<int>>> result;
    do {
        std::vector<int> first, second;
        for (int i = 0; i < n; ++i) {
            if (v[i] == 2)
                first.push_back(i);
            else if (v[i] == 1)
                second.push_back(i);
        }
        result.push_back({first, second});
    } while (std::next_permutation(v.begin(), v.end()));
    return result;
}