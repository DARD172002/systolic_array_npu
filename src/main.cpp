#include "../include/array_systolic.hpp"
#include <iostream>

void printMatrix(const std::vector<std::vector<DataType>>& matrix) {
    for (const auto& row : matrix) {
        for (auto val : row)
            std::cout << val << "\t";
        std::cout << "\n";
    }
}

int main() {
    SystolicArray array(4);

    std::vector<std::vector<DataType>> A = {
        {1, 2, 0, 0},
        {0, 3, 4, 0},
        {5, 6, 0, 0},
        {0, 0, 0, 0}
    };

    std::vector<std::vector<DataType>> B = {
        {7, 8, 0, 0},
        {0, 9, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    auto result = array.compute(A, B);

    std::cout << "Resultado del arreglo sistólico:\n";
    printMatrix(result);

    return 0;
}
