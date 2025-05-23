

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>

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
    DataType a_reg = 0;
    DataType b_reg = 0;
    DataType acc = 0;
    bool valid = false;

    void step(const DataType& a_in, const DataType& b_in, bool a_valid, bool b_valid, PerformanceCounters& perf) {
        if (a_valid && b_valid) {
            a_reg = a_in;
            b_reg = b_in;
            acc += a_reg * b_reg;
            valid = true;
            perf.macs++;
            perf.memory_accesses += 2;
        } else {
            valid = false;
        }
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
        const std::vector<std::vector<DataType>>& A,
        const std::vector<std::vector<DataType>>& B
    ) {
        const int total_cycles = 2 * N + (N - 2);  

        // Buffers para propagación
        std::vector<std::vector<DataType>> a_pipe(N, std::vector<DataType>(N, 0));
        std::vector<std::vector<DataType>> b_pipe(N, std::vector<DataType>(N, 0));
        std::vector<std::vector<bool>> a_valid(N, std::vector<bool>(N, false));
        std::vector<std::vector<bool>> b_valid(N, std::vector<bool>(N, false));

        for (int t = 0; t < total_cycles; ++t) {
            // Movimiento y entrada de datos
            for (int i = N - 1; i >= 0; --i) {
                for (int j = N - 1; j >= 0; --j) {
                    // Datos a_in
                    DataType a_in = 0;
                    bool a_in_valid = false;
                    if (j == 0) {
                        if (t - i >= 0 && t - i < N) {
                            a_in = A[i][t - i];
                            a_in_valid = true;
                        }
                    } else {
                        a_in = a_pipe[i][j - 1];
                        a_in_valid = a_valid[i][j - 1];
                    }

                    // Datos b_in
                    DataType b_in = 0;
                    bool b_in_valid = false;
                    if (i == 0) {
                        if (t - j >= 0 && t - j < N) {
                            b_in = B[t - j][j];
                            b_in_valid = true;
                        }
                    } else {
                        b_in = b_pipe[i - 1][j];
                        b_in_valid = b_valid[i - 1][j];
                    }

                    pes[i][j].step(a_in, b_in, a_in_valid, b_in_valid, perf);

                    // Guardar en pipe para el próximo PE
                    a_pipe[i][j] = pes[i][j].a_reg;
                    b_pipe[i][j] = pes[i][j].b_reg;
                    a_valid[i][j] = a_in_valid;
                    b_valid[i][j] = b_in_valid;
                }
            }
        }

        // Armar salida con ReLU
        std::vector<std::vector<DataType>> C(N, std::vector<DataType>(N));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                C[i][j] = pes[i][j].activate();

        perf.memory_accesses += N * N; // Escritura del resultado
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

// Funciones auxiliares
std::vector<std::vector<DataType>> cargarMatriz(const std::string& nombreArchivo) {
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

void imprimirMatriz(const std::vector<std::vector<DataType>>& matriz) {
    for (const auto& fila : matriz) {
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
