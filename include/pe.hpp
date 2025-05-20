#pragma once
#include <cstdint>

using DataType = int16_t;

struct PE {
    DataType partial_sum = 0;

    DataType compute(DataType input, DataType weight) {
        partial_sum += input * weight;
        return relu(partial_sum);
    }

    void reset() { partial_sum = 0; }
   


    DataType relu(DataType x) {
        return x > 0 ? x : 0;
    }
};
