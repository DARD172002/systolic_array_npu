`timescale 1ns / 1ps

module PE_tb;

    parameter DATA_WIDTH = 16;
    logic clk, rst;
    logic [DATA_WIDTH-1:0] a_in, b_in;
    logic valid_in;
    logic [DATA_WIDTH-1:0] a_out, b_out, result;
    logic valid_out;

    PE #(.DATA_WIDTH(DATA_WIDTH)) dut (
        .clk(clk),
        .rst(rst),
        .a_in(a_in),
        .b_in(b_in),
        .valid_in(valid_in),
        .a_out(a_out),
        .b_out(b_out),
        .result(result),
        .valid_out(valid_out)
    );

    // Clock generation
    always #5 clk = ~clk;

    initial begin
        $display("==== INICIO DE SIMULACIÓN ====");
        clk = 0;
        rst = 1;
        a_in = 0; b_in = 0; valid_in = 0;
        #10 rst = 0;

        // Ciclo 1: 10 * 3 = 30
        @(posedge clk);
        a_in = 10;
        b_in = 3;
        valid_in = 1;

        // Ciclo 2: -5 * 4 = -20
        @(posedge clk);
        a_in = -5;
        b_in = 4;
        valid_in = 1;

        // Ciclo 3: No operación
        @(posedge clk);
        valid_in = 0;
        a_in = 0;
        b_in = 0;

        // +1 ciclo para actaulizar a result
        @(posedge clk);

        $display("Resultado acumulado (ReLU): %0d", result);  
        if (result == 10)
            $display("Test PASSED");
        else
            $display("Test FAILED");

        $stop;
    end

endmodule

