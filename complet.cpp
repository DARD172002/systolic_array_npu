

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>

// Tipo de dato con signo de 16 bits
using DataType = int16_t;

constexpr int N = 4;

struct PerformanceCounters {
    int macs = 0;
    int memory_accesses = 0;
    bool finished = false;
};

DataType relu(DataType x) {
    return x > 0 ? x : 0;
}

class ProcessingElement {
public:
    DataType acc = 0;

    void compute(DataType a, DataType b, PerformanceCounters &perf) {
        acc += a * b;
        perf.macs++;
        perf.memory_accesses += 2; // a y b
    }

    DataType activate() const {
        return relu(acc);
    }
};

class SystolicArray {
private:
    std::vector<std::vector<ProcessingElement>> pes;
    PerformanceCounters perf;

public:
    SystolicArray() : pes(N, std::vector<ProcessingElement>(N)) {}

    std::vector<std::vector<DataType>> compute(
        const std::vector<std::vector<DataType>> &A,
        const std::vector<std::vector<DataType>> &B
    ) {
        for (int t = 0; t < 2 * N - 1; ++t) {
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    int k = t - i;
                    if (k >= 0 && k < N) {
                        pes[i][j].compute(A[i][k], B[k][j], perf);
                    }
                }
            }
        }

        std::vector<std::vector<DataType>> C(N, std::vector<DataType>(N));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                C[i][j] = pes[i][j].activate();

        perf.memory_accesses += N * N; // Escrituras
        perf.finished = true;
        return C;
    }

    void printStats() const {
        std::cout << "\n--- REGISTROS DE ESTADO ---\n";
        std::cout << "MACs ejecutadas: " << perf.macs << "\n";
        std::cout << "Accesos a memoria: " << perf.memory_accesses << "\n";
        std::cout << "Cómputo finalizado: " << (perf.finished ? "Sí" : "No") << "\n";
    }
};

std::vector<std::vector<DataType>> cargarMatriz(const std::string &nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::vector<std::vector<DataType>> matriz(N, std::vector<DataType>(N));
    std::string linea;
    int fila = 0;

    while (std::getline(archivo, linea) && fila < N) {
        std::istringstream ss(linea);
        for (int col = 0; col < N; ++col)
            ss >> matriz[fila][col];
        ++fila;
    }

    return matriz;
}

void imprimirMatriz(const std::vector<std::vector<DataType>> &matriz) {
    for (const auto &fila : matriz) {
        for (DataType val : fila)
            std::cout << std::setw(8) << val;
        std::cout << "\n";
    }
}

int main() {
    auto A = cargarMatriz("files/matrizA.txt");
    auto B = cargarMatriz("files/matrizB.txt");

    std::cout << "Matriz A:\n";
    imprimirMatriz(A);
    std::cout << "\nMatriz B:\n";
    imprimirMatriz(B);

    SystolicArray sa;
    auto C = sa.compute(A, B);

    std::cout << "\nResultado C = ReLU(A * B):\n";
    imprimirMatriz(C);

    sa.printStats();
    return 0;
}
