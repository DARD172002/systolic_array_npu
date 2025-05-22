#include <iostream>
#include <iomanip>

#include <cstdint>


using namespace std;





//defined fixed size 
using int16=uint16_t;

//parameter

const int N = 4;





// activation function
float relu(float x) {
    return x > 0 ? x : 0;
}


//control register

struct  ControlRegister{
    bool start=false;
    int cycle=0;
    bool step_mode=false;

};


// simulated of register
struct SimulatedRegister{
    int mac_operations = 0;
    int memory_accesses = 0;
    bool done = false;


};




class PE {
public:
    float a = 0;
    float b = 0;
    float acc = 0;
    bool valid = false;

    void cycle() {
        if (valid)
            acc += a * b;
    }

    float get_output() const {
        return relu(acc); // activación no lineal
    }
};

class SystolicArray {
private:
    PE pes[N][N];
    float a_regs[N][N] = {0};   // registros locales para propagar A
    float b_regs[N][N] = {0};   // registros locales para propagar B
    bool valid[N][N] = {false};

public:
    void run(float A[N][N], float B[N][N]) {
        const int total_cycles = 2 * N + N - 2;

        for (int t = 0; t < total_cycles; ++t) {
            // Propagar datos en columnas (A) y filas (B)
            for (int i = N - 1; i >= 0; --i) {
                for (int j = N - 1; j >= 0; --j) {
                    if (i > 0) {
                        a_regs[i][j] = a_regs[i - 1][j];
                        valid[i][j] = valid[i - 1][j];
                    }
                    if (j > 0) {
                        b_regs[i][j] = b_regs[i][j - 1];
                    }
                }
            }

          for (int j = 0; j < N; ++j) {
    if (t - j >= 0 && t - j < N) {
        a_regs[0][j] = A[t - j][j];  // fila t-j, columna j
        valid[0][j] = true;
    }
}
for (int i = 0; i < N; ++i) {
    if (t - i >= 0 && t - i < N) {
        b_regs[i][0] = B[i][t - i];  // fila i, columna t-i
    }
}
            // Cargar registros en cada PE y ejecutar ciclo
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j) {
                    pes[i][j].a = a_regs[i][j];
                    pes[i][j].b = b_regs[i][j];
                    pes[i][j].valid = valid[i][j];
                    pes[i][j].cycle();
                }
        }
    }

    void print_output() const {
        std::cout << "Resultado C = ReLU(A * B):\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j)
                std::cout << std::setw(6) << pes[i][j].get_output() << " ";
            std::cout << "\n";
        }
    }
};

int main() {
    float A[N][N] = {
        {1, 2, 3, 0},
        {4, 5, 6, 0},
        {7, 8, 9, 0},
        {0, 0, 0, 0}
    };

    float B[N][N] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    };

    SystolicArray array;
    array.run(A, B);
    array.print_output();

    return 0;
}
