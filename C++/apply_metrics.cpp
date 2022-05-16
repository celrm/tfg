#include "include/lib.h"

#define V std::vector<int>
#define M std::vector<V>
#define R std::vector<std::string>

static int N = 5;
static bool is_n = false;


static R get_chars(std::vector<BitT> const& row) {
    R result;
    for (auto const& b : row) {
        std::string s = std::to_string(b);
        result.push_back(s);
    }
    return result;
}

// get_bits
static std::vector<BitT> get_bits(R const& row) {
    std::vector<BitT> result;
    for (int i = 2; i < row.size(); ++i)
        result.push_back(row[i][0] - '0');
    return result;
}

static std::vector<BitT> functionToBits(std::string fstr) {
    ExpT function = std::stoll(fstr);
    std::vector<BitT> result;
    while (function > 0) {
        result.push_back(function % 2);
        function /= 2;
    }
    if(result.size() < (1<<N))
        result.resize((1<<N), 0);
    std::reverse(result.begin(), result.end());
    return result;
}


int main(int argc,  char** argv) {
    if(argc != 3) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        std::cout << "Example: " << argv[0] << " input.txt output.txt" << std::endl;
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644); // "result.csv"
    dup2(fd, 1);

    std::ifstream ifs;
    ifs.open(argv[1], std::ifstream::in); // be careful with size // "./bitsDataset.csv"

    R row = get_row(ifs); // header

    if(is_n) print_row(row);
    else print_row({row[0],row[1]});

    for (int k = 0; k < N; ++k)
        std::cout << ",chunks" << k;
    std::cout << ",ct";
    std::cout << ",cube";
    std::cout << std::endl;

    while (ifs.good()) {
        row = get_row(ifs);
        if (row.size() < 2) continue;
        if (row[1] == "Small" || row[1] == "0") row[1] = "0";
        else if (row[1] == "Big" || row[1] == "1") row[1] = "1";

        if(is_n) print_row(row);
        else print_row({row[0],row[1]});

        FunctionT bits;
        if(is_n) bits = get_bits(row);
        else bits = functionToBits(row[0]);

        std::vector<FunctionT> sigma_chains = all_sigma_chains(bits);
        for (int k = 0; k < N; ++k) {
            int min_chunks_k = INT_MAX;
            for (FunctionT const& sigma_chain : sigma_chains) {
                int chunks_k = disorder_chunks_k(k, sigma_chain);
                min_chunks_k = std::min(min_chunks_k, chunks_k);
            }
            std::cout << "," << min_chunks_k;
        }

        int min_CT = INT_MAX;
        for (FunctionT const& sigma_chain : sigma_chains) {
            int CT = disorder_CT(sigma_chain);
            min_CT = std::min(min_CT, CT);
        }
        std::cout << "," << min_CT;

        std::cout << "," << disorder_cube(bits);
        std::cout << std::endl;
    }
    ifs.close();
}