#include <iostream>
#include <fstream>
#include <random>
#include <set>

typedef unsigned long long ull;
int main() {
    std::mt19937_64 rng;
    rng.seed(std::random_device()());

    const size_t array_size = 3 * 19 * 19 + 1000;
    std::set<ull> unique_values;

    // 生成所有可能唯一的值
    while (unique_values.size() < array_size) {
        unique_values.insert(rng());
    }

    // 填充三维数组
    ull a[3][19][19];
    size_t index = 0;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 19; ++j) {
            for (size_t k = 0; k < 19; ++k) {
                a[i][j][k] = *std::next(unique_values.begin(), index++);
            }
        }
    }

    // 将数组输出到文件
    std::ofstream outfile("output.txt");
    if (outfile.is_open()) {
        outfile << "const int ZOBRIST_CODE[3][19][19] = {\n";
        for (int i = 0; i < 3; ++i) {
            outfile << "    {\n";
            for (int j = 0; j < 19; ++j) {
                outfile << "        {";
                for (int k = 0; k < 19; ++k) {
                    outfile << a[i][j][k];
                    if (k < 19 - 1) {
                        outfile << ", ";
                    }
                }
                outfile << "}";
                if (j < 19 - 1) {
                    outfile << ",";
                }
                outfile << "\n";
            }
            outfile << "    }";
            if (i < 3 - 1) {
                outfile << ",";
            }
            outfile << "\n";
        }
        outfile << "};\n";
        outfile.close();
        std::cout << "Array data written to output.txt.\n";
    }
    else {
        std::cerr << "Error opening output file!\n";
    }

    return 0;
}
