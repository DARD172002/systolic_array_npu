

#pragma once
#include "pe.hpp"
#include <vector>

class SystolicArray {
    size_t size;
    std::vector<std::vector<PE>> pes;

public:
    SystolicArray(size_t n) : size(n), pes(n, std::vector<PE>(n)) {}

    void reset() {
        for (auto& row : pes)
            for (auto& pe : row)
                pe.reset();
    }

    std::vector<std::vector<DataType>> compute(
        const std::vector<std::vector<DataType>>& A,
        const std::vector<std::vector<DataType>>& B
    ) {
        size_t N = A.size();
        size_t M = B[0].size();
        size_t K = A[0].size();

        std::vector<std::vector<DataType>> result(N, std::vector<DataType>(M, 0));

        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                DataType sum = 0;
                for (size_t k = 0; k < K; ++k) {
                    sum += A[i][k] * B[k][j];
                }
                result[i][j] = pes[i][j].relu(sum);
            }
        }

        return result;
    }
};
