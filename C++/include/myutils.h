#ifndef MYUTILS_H_
#define MYUTILS_H_
#include <algorithm>

int print_row(std::vector<std::string> const &row, std::string const &delim = ",", std::string const &final = "\n");

std::vector<std::string> get_row(std::ifstream &ifs);

std::vector<std::vector<std::vector<int>>> combinations(int n, std::vector<int> const &n_s);

std::vector<std::vector<int>> negative_combinations(int (*neg)(int), std::vector<int> const &s);

#endif // MYUTILS_H_
