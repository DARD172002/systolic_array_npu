module tb_systolic_array_4x4;

    parameter N = 4;
    parameter DATA_WIDTH = 16;

    logic clk = 0;
    logic rst = 1;
    logic signed [DATA_WIDTH-1:0] A_in[N], B_in[N];
    logic valid_in;
    logic signed [DATA_WIDTH-1:0] result[N][N];
    logic valid_out[N][N];

    systolic_array_4x4 #(DATA_WIDTH, N) uut (
        .clk(clk),
        .rst(rst),
        .A_in(A_in),
        .B_in(B_in),
        .valid_in(valid_in),
        .result(result),
        .valid_out(valid_out)
    );

    always #5 clk = ~clk;

    initial begin
        $display("== Iniciando prueba ==");

        // Reset
        #10 rst = 0;

        // Ciclo 0
        A_in[0] = 1;  B_in[0] = 1;
        A_in[1] = 5;  B_in[1] = 0;
        A_in[2] = 9;  B_in[2] = 0;
        A_in[3] = 13; B_in[3] = 0;
        valid_in = 1; #10;

        // Ciclo 1
        A_in[0] = 2;  B_in[0] = 0;
        A_in[1] = 6;  B_in[1] = 1;
        A_in[2] = 10; B_in[2] = 0;
        A_in[3] = 14; B_in[3] = 0;
        #10;

        // Ciclo 2
        A_in[0] = 3;  B_in[0] = 0;
        A_in[1] = 7;  B_in[1] = 0;
        A_in[2] = 11; B_in[2] = 1;
        A_in[3] = 15; B_in[3] = 0;
        #10;

        // Ciclo 3
        A_in[0] = 4;  B_in[0] = 0;
        A_in[1] = 8;  B_in[1] = 0;
        A_in[2] = 12; B_in[2] = 0;
        A_in[3] = 16; B_in[3] = 1;
        #10;

        valid_in = 0;
        #100;

        $display("== RESULTADO ==");
        for (int i = 0; i < N; i++) begin
            $display("%d %d %d %d", result[i][0], result[i][1], result[i][2], result[i][3]);
        end

        $stop;
    end
endmodule
