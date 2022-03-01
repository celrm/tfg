#ifndef MYUTILS_H_
#define MYUTILS_H_
#include <algorithm>

int print_row(std::vector<std::string> const &row, std::string const &delim = ",", std::string const &final = "\n");

std::vector<std::string> get_row(std::ifstream &ifs);

std::vector<std::pair<std::vector<int>, std::vector<int>>> combinations(int n, std::pair<int, int> const &n_s);

#endif // MYUTILS_H_
