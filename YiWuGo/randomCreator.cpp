#include <iostream>
#include <fstream>
#include <random>

int main() {
    // 使用 mt19937_64 随机数生成器
    std::mt19937_64 rng;

    // 设置随机数种子，可以使用当前时间作为种子
    rng.seed(std::random_device()());

    // 定义三维数组
    const int rows = 3;
    const int cols = 19;
    const int depth = 19;
    unsigned long long a[rows][cols][depth];

    // 填充三维数组
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < depth; ++k) {
                a[i][j][k] = rng(); // 生成随机数并填充数组
            }
        }
    }

    // 将数组输出到文件
    std::ofstream outfile("output.txt");
    if (outfile.is_open()) {
        outfile << "const int ZOBRIST_CODE[3][19][19] = {\n";
        for (int i = 0; i < rows; ++i) {
            outfile << "    {\n";
            for (int j = 0; j < cols; ++j) {
                outfile << "        {";
                for (int k = 0; k < depth; ++k) {
                    outfile << a[i][j][k];
                    if (k < depth - 1) {
                        outfile << ", ";
                    }
                }
                outfile << "}";
                if (j < cols - 1) {
                    outfile << ",";
                }
                outfile << "\n";
            }
            outfile << "    }";
            if (i < rows - 1) {
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
