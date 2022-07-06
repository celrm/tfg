#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "include/lib.h"
#define R std::vector<std::string>
const LenT N = 3; // 5 in 1 bit functions


int main() {
    srand(time(NULL));

    int fd = open("result.csv", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    dup2(fd, 1);

    ExpT circuits_per_size = 1000;
    LenT max_size = 31;

    printf("w,i,e1,e2,m,m1,m2,a\n");
    for (LenT size = 3; size <= max_size; size+=2) {
        CircuitT c1(size, std::vector<std::vector<LenT>>(size, std::vector<LenT>(2, 0)));
        CircuitT c2(size, std::vector<std::vector<LenT>>(size, std::vector<LenT>(2, 0)));

        for (ExpT i = 0; i < circuits_per_size; ++i) {
            random_circuit(c1, N);
            random_circuit(c2, N);
            
            std::vector<FunctionT> f1(size, FunctionT(1 << N, 0));
            std::vector<FunctionT> f2(size, FunctionT(1 << N, 0));
            int e1 = mutual_endogamy(c1, c2, f1, f2, N);
            double e2 = mutual_endogamy2(c1, c2, f1, f2, N);

            FunctionT f1_ = f1[0];
            FunctionT f2_ = f2[0];
            FunctionT f(1 << N, 0);
            for (LenT i = 0; i < (1<<N); ++i) {
                f[i] = 1 ^ (f1_[i] & f2_[i]);
            }

            std::vector<std::vector<int>> questions1(N);
            std::vector<std::vector<int>> questions2(N);
            int m1 = disorder_cube2(f1_, questions1);
            int m2 = disorder_cube2(f2_, questions2);
            int m = disorder_cube(f);

            int a = mean_question_height(questions1, questions2);

            printf("%d,%lld,%d,%f,%d,%d,%d,%d\n", size, i, e1, e2, m, m1, m2, a);
        }
    }
}