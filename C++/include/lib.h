#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h> // \n( )*\{
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#define BitT char
#define LenT char
#define ExpT long long
#define CircuitT std::vector<std::vector<std::vector<LenT>>>
#define MatrixT std::vector<std::vector<LenT>>
#define FunctionT std::vector<BitT>

#include "../include/metrics.h"
#include "../include/circuit_gen.h"
#include "../include/myutils.h"

#endif // UTILS_H_